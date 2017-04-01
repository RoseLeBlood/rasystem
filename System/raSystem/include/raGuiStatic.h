#pragma once

namespace raSystem
{
	class raGuiStatic : public raGuiControl
	{
	public:
		raGuiStatic( raGuiDialog* pDialog = NULL );

		virtual void    Render( float fTime );
		virtual BOOL    ContainsPoint( POINT pt )		{ return false; }

		HRESULT         GetTextCopy( __out_ecount(bufferCount) LPCSTR strDest,  UINT bufferCount );
		LPCSTR          GetText() { return m_strText; }

		HRESULT         SetText( LPCSTR strText );


	protected:
		char           m_strText[MAX_PATH];       
	};
}

