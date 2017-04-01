#pragma once

namespace raSystemGui
{
	template <class T> class _RestrictedFileDialogPointer : public T
    {
    private:
        STDMETHOD_(ULONG, AddRef)() = 0;
        STDMETHOD_(ULONG, Release)() = 0;
        STDMETHOD(Show)(HWND) = 0;
    };

    template <typename Interface, REFCLSID ClassId> class FileDialog : protected CWindow,
        private IFileDialogEvents,
        private IFileDialogControlEvents
    {
    public:
        __checkReturn HRESULT Load()
        {
            HRESULT result = m_dialog.CoCreateInstance(ClassId);

            if (SUCCEEDED(result))
            {
                DWORD cookie = 0;

                result = m_dialog->Advise(this,
                                          &cookie);

                if (SUCCEEDED(result))
                {
                    result = m_dialog.QueryInterface(&m_customize);
                }
            }

            return result;
        }

        HRESULT DoModal(HWND parent = ::GetActiveWindow())
        {
            ASSERT(0 == m_hWnd);

            HRESULT result = m_dialog->Show(parent);

            ASSERT(0 != m_hWnd);
            m_hWnd = 0;

            return result;
        }

        _RestrictedFileDialogPointer<Interface>* operator->() const
        {
            ASSERT(0 != m_dialog.p);
            return static_cast<_RestrictedFileDialogPointer<Interface>*>(m_dialog.p);
        }

    protected:
        virtual bool OnFileOk()
        {
            return true;
        }

        virtual bool OnFolderChanging(IShellItem* shellItem)
        {
            return true;
        }
        virtual void OnFolderChange(IShellItem* shellItem)
        {
        }

        virtual void OnSelectionChange()
        {
        }

        virtual FDE_SHAREVIOLATION_RESPONSE OnShareViolation(IShellItem* shellItem)
        {
            return FDESVR_DEFAULT;
        }

        virtual void OnTypeChange(UINT index)
        {
        }

        virtual FDE_OVERWRITE_RESPONSE OnOverwrite(IShellItem* shellItem)
        {
            return FDEOR_DEFAULT;
        }
        virtual void OnItemSelected(DWORD controlId, DWORD itemId)
        {
        }

        virtual void OnButtonClicked(DWORD controlId)
        {
        }

        virtual void OnCheckButtonToggled(DWORD controlId, bool checked)
        {
        }

        virtual void OnControlActivating(DWORD /*controlId*/)
        {
        }
        CComPtr<Interface> m_dialog;
        CComPtr<IFileDialogCustomize> m_customize;

    private:

        __checkReturn HRESULT LoadWindowHandle()
        {
            HRESULT result = S_OK;

            if (0 == m_hWnd)
            {
                CComPtr<IOleWindow> window;
                result = m_dialog.QueryInterface(&window);

                if (SUCCEEDED(result))
                {
                    result = window->GetWindow(&m_hWnd);
                }
            }

            ASSERT(0 != m_hWnd);
            ASSERT(SUCCEEDED(result));

            return result;
        }
        IFACEMETHODIMP QueryInterface(REFIID iid, void** object)
        {
            if (__uuidof(IUnknown) == iid || __uuidof(IFileDialogEvents) == iid)
            {
                *object = static_cast<IFileDialogEvents*>(this);
            }
            else if (__uuidof(IFileDialogControlEvents) == iid)
            {
                *object = static_cast<IFileDialogControlEvents*>(this);
            }
            else
            {
                *object = 0;
            }

            return (0 != *object) ? S_OK : E_NOINTERFACE;
        }

        IFACEMETHODIMP_(ULONG) AddRef()
        {
            return 1;
        }

        IFACEMETHODIMP_(ULONG) Release()
        {
            return 0;
        }
        IFACEMETHODIMP OnFileOk(IFileDialog* dialog)
        {
            ASSERT(0 != dialog);

            HRESULT result = 0 != dialog ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    result = OnFileOk() ? S_OK : S_FALSE;
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnFolderChanging(IFileDialog* dialog,
                                        IShellItem* shellItem)
        {
            ASSERT(0 != dialog);
            ASSERT(0 != shellItem);

            HRESULT result = 0 != dialog && 0 != shellItem ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    result = OnFolderChanging(shellItem) ? S_OK : S_FALSE;
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnFolderChange(IFileDialog* dialog)
        {
            ASSERT(0 != dialog);

            HRESULT result = 0 != dialog ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    CComPtr<IShellItem> shellItem;
                    result = dialog->GetFolder(&shellItem);

                    if (SUCCEEDED(result))
                    {
                        OnFolderChange(shellItem);
                    }
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnSelectionChange(IFileDialog* dialog)
        {
            ASSERT(0 != dialog);

            HRESULT result = 0 != dialog ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    OnSelectionChange();
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnShareViolation(IFileDialog* dialog,
                                        IShellItem* shellItem,
                                        FDE_SHAREVIOLATION_RESPONSE* response)
        {
            ASSERT(0 != dialog);
            ASSERT(0 != shellItem);
            ASSERT(0 != response);

            HRESULT result = 0 != dialog && 0 != shellItem && 0 != response ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    *response = OnShareViolation(shellItem);
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnTypeChange(IFileDialog* dialog)
        {
            ASSERT(0 != dialog);

            HRESULT result = 0 != dialog ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    UINT index = 0;
                    result = dialog->GetFileTypeIndex(&index);

                    if (SUCCEEDED(result))
                    {
                        OnTypeChange(index);
                    }
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnOverwrite(IFileDialog* dialog,
                                   IShellItem* shellItem,
                                   FDE_OVERWRITE_RESPONSE* response)
        {
            ASSERT(0 != dialog);
            ASSERT(0 != shellItem);
            ASSERT(0 != response);

            HRESULT result = 0 != dialog && 0 != shellItem && 0 != response ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    *response = OnOverwrite(shellItem);
                }
            }

            COM_VERIFY(result);
            return result;
        }

        //
        // IFileDialogControlEvents methods
        //

        IFACEMETHODIMP OnItemSelected(IFileDialogCustomize* customize,
                                      DWORD controlId,
                                      DWORD itemId)
        {
            ASSERT(0 != customize);

            HRESULT result = 0 != customize ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    OnItemSelected(controlId,
                                   itemId);
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnButtonClicked(IFileDialogCustomize* customize,
                                       DWORD controlId)
        {
            ASSERT(0 != customize);

            HRESULT result = 0 != customize ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    OnButtonClicked(controlId);
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnCheckButtonToggled(IFileDialogCustomize* customize,
                                            DWORD controlId,
                                            BOOL checked)
        {
            ASSERT(0 != customize);

            HRESULT result = 0 != customize ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    OnCheckButtonToggled(controlId,
                                         0 != checked);
                }
            }

            COM_VERIFY(result);
            return result;
        }

        IFACEMETHODIMP OnControlActivating(IFileDialogCustomize* customize,
                                           DWORD controlId)
        {
            ASSERT(0 != customize);

            HRESULT result = 0 != customize ? S_OK : E_POINTER;

            if (SUCCEEDED(result))
            {
                result = LoadWindowHandle();

                if (SUCCEEDED(result))
                {
                    OnControlActivating(controlId);
                }
            }

            COM_VERIFY(result);
            return result;
        }
    };

    typedef FileDialog<IFileSaveDialog, __uuidof(FileSaveDialog)> FileSaveDialog;
    typedef FileDialog<IFileOpenDialog, __uuidof(FileOpenDialog)> FileOpenDialog;
}