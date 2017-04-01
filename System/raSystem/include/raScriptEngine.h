#pragma managed

#using <mscorlib.dll>
#using <system.dll>


using namespace System;
using namespace System::IO;
using namespace System::Reflection;
using namespace System::CodeDom;
using namespace System::CodeDom::Compiler;

using namespace System::Security;
using namespace System::Security::Policy;
using namespace System::Security::Permissions;

using namespace Microsoft::CSharp;


#define SCRIPT_TYPE_NAME "raScriptClass"
#define SCRIPT_UPDATE_TEST "UpdatePosition"

#ifdef GetTempFileName
#undef GetTempFileName
#endif

namespace raSystem
{
	public ref class raScriptEngine sealed
	{
	public:
		static bool Create()
		{
			return true;
		}
		static void Destroy()
		{

		}
		static void ResetObjects()
		{
			m_pScriptAssembly = nullptr;
			m_pScriptType = nullptr;
			m_hasNotSecurityException = false;

			if((m_pSavedFile != nullptr ) && (m_pSavedFile->Length > 0))
			{
				try
				{
					File::Delete(m_pSavedFile);
				}
				catch(Exception^)
				{
				}
				m_pSavedFile = nullptr;
			}
		}
		static bool LoadScriptData(System::String^ pScriptName)
		{
			ResetObjects();

			if(pScriptName == nullptr || pScriptName->Length == 0)
				return false;

			StreamReader^ pReader = nullptr;
			try
			{
				pReader = gcnew StreamReader(pScriptName);
				System::String^ pScriptText = pReader->ReadToEnd();

				if(pScriptText == nullptr || pScriptText->Length == 0)
					return false;

				CSharpCodeProvider^ pProvider = gcnew CSharpCodeProvider();
				CompilerParameters^ pParams = gcnew CompilerParameters();
				pParams->GenerateExecutable = false;
				pParams->GenerateInMemory = false;
				m_pSavedFile = System::String::Concat(Path::GetTempFileName(), ".dll");
				pParams->OutputAssembly = m_pSavedFile;

				CompilerResults^ pResult = pProvider->CompileAssemblyFromSource(pParams, 
					gcnew array<System::String^>{ pScriptText });

				if(pResult->Errors->Count > 0)
				{
					System::Diagnostics::Debugger::Log(0, nullptr,
						pResult->Errors[0]->ErrorText);
					return false;
				}
				else
				{
					m_pScriptAssembly = System::Reflection::Assembly::LoadFrom(pResult->PathToAssembly);
					m_pScriptType = m_pScriptAssembly->GetType(SCRIPT_TYPE_NAME, false, true);
					if(m_pScriptType = nullptr)
					{
						m_pScriptAssembly = nullptr;
						return false;
					}
					return true;
				}
			}
			catch(Exception^ e)
			{
				System::Diagnostics::Debugger::Log(0, nullptr,
						e->ToString());
				ResetObjects();
				return false;
			}
			__finally
			{
				delete pReader;
			}
		}
		static bool UpdatePosition(float fRunTime, float *pX, float *pY, float *pZ)
		{
			if(pX == NULL || pY == NULL || pZ == NULL)
				return false;
			if(m_pScriptAssembly == nullptr || m_pScriptType == nullptr)
				return false;

			if(!m_canUpdatePosition)
				return false;

			try
			{
				array<System::Object^>^pParams =  { fRunTime, *pX, *pY, *pZ };
				m_pScriptType->InvokeMember(SCRIPT_UPDATE_TEST, (BindingFlags)(
					BindingFlags::InvokeMethod |
					BindingFlags::DeclaredOnly |
					BindingFlags::Public |
					BindingFlags::NonPublic |
					BindingFlags::Static),
					nullptr, nullptr, pParams);

				*pX = static_cast<float>(pParams[1]);
				*pY = static_cast<float>(pParams[2]);
				*pZ = static_cast<float>(pParams[3]); 
			}
			catch(TargetInvocationException^ e)
			{
				System::Diagnostics::Debugger::Log(0, nullptr,
						e->ToString());
				m_canUpdatePosition = false;
				return false;
			}
			return true;
		}
	private:
		static Assembly^	m_pScriptAssembly = nullptr;
		static Type^		m_pScriptType = nullptr;
		static String^		m_pSavedFile = nullptr;
		static bool			m_hasNotSecurityException = false;
		static bool			m_canUpdatePosition = true;

	};
}