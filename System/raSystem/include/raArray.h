// DXUT CGrowableArray ist das hier, naja meine version war sehr verbugt.

	template<typename TYPE> class raArray
	{
	public:
		raArray()   { m_pData = NULL; m_nSize = 0; m_nMaxSize = 0; }
		//raArray( const raArray<TYPE>& a ) : raClass("raArray")  { for( int i=0; i < int(a.m_nSize); i++ ) Add( a.m_pData[i] ); }
		~raArray() { RemoveAll(); }

		const TYPE& operator[]( raInt nIndex ) const { return GetAt( (int)nIndex ); }
		TYPE& operator[]( raInt nIndex ) { return GetAt( (int)nIndex ); }

		raArray& operator=( const raArray<TYPE>& a ) { if( this == &a ) return *this; RemoveAll(); for( int i=0; i < a.m_nSize; i++ ) Add( a.m_pData[i] ); return *this; }

		HRESULT SetSize( raInt nNewMaxSize );
		HRESULT Add( const TYPE& value );
		HRESULT Insert( raInt nIndex, const TYPE& value );
		HRESULT SetAt( raInt nIndex, const TYPE& value );
		TYPE&   GetAt( raInt nIndex ) const { assert( (int)nIndex >= 0 && (int)nIndex < (int)m_nSize ); return m_pData[(int)nIndex]; }
		raInt     GetSize() const { return m_nSize; }
		TYPE*   GetData() { return m_pData; }
		bool    Contains( const TYPE& value ){ return ( -1 != IndexOf( value ) ); }

		raInt     IndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? IndexOf( value, 0, m_nSize ) : -1; }
		raInt     IndexOf( const TYPE& value, raInt iStart ) { return IndexOf( value, iStart, m_nSize - iStart ); }
		raInt     IndexOf( const TYPE& value, raInt nIndex, raInt nNumElements );

		raInt     LastIndexOf( const TYPE& value ) { return ( m_nSize > 0 ) ? LastIndexOf( value, m_nSize-1, m_nSize ) : -1; }
		raInt     LastIndexOf( const TYPE& value, raInt nIndex ) { return LastIndexOf( value, nIndex, nIndex+1 ); }
		raInt     LastIndexOf( const TYPE& value, raInt nIndex, raInt nNumElements );

		HRESULT Remove( raInt nIndex );
		void    RemoveAll() { SetSize(0); }
		void	Reset() { m_nSize = 0; }

	protected:
		virtual void __vsforeach( void (* func)(TYPE))
		{
			for(int i= 0; i < (int)GetSize(); i++)
			{
				if(func) func (GetAt(i));
			}
		}

		HRESULT SetSizeInternal( int nNewMaxSize );
	protected:
		TYPE* m_pData;
		raInt m_nSize;
		raInt m_nMaxSize;
	};
	template<typename TYPE> HRESULT raArray <TYPE>::SetSizeInternal( int nNewMaxSize )
	{
		if( nNewMaxSize < 0 || ( nNewMaxSize > INT_MAX / sizeof( TYPE ) ) )
		{
			assert( false );
			return E_INVALIDARG;
		}

		if( nNewMaxSize == 0 )
		{
			if( m_pData )
			{
				free( m_pData );
				m_pData = NULL;
			}

			m_nMaxSize = 0;
			m_nSize = 0;
		}
		else if( m_pData == NULL || nNewMaxSize > m_nMaxSize )
		{
			raInt nGrowBy = ( m_nMaxSize == 0 ) ? 16 : m_nMaxSize;

			if( ( UINT )m_nMaxSize + ( UINT )nGrowBy > ( UINT )INT_MAX )
				nGrowBy = INT_MAX - m_nMaxSize;

			nNewMaxSize = __max( nNewMaxSize, m_nMaxSize + nGrowBy );

			if( sizeof( TYPE ) > UINT_MAX / ( UINT )nNewMaxSize )
				return E_INVALIDARG;

			TYPE* pDataNew = ( TYPE* )std::realloc( m_pData, nNewMaxSize * sizeof( TYPE ) );
			if( pDataNew == NULL )
				return E_OUTOFMEMORY;

			m_pData = pDataNew;
			m_nMaxSize = nNewMaxSize;
		}

		return S_OK;
	}
	template<typename TYPE> HRESULT raArray <TYPE>::SetSize( raInt nNewMaxSize )
	{
		raInt nOldSize = m_nSize;

		if( (int)nOldSize > (int)nNewMaxSize )
		{
			assert( m_pData );
			if( m_pData )
			{
				for( int i = (int)nNewMaxSize; i < (int)nOldSize; ++i )
					m_pData[i].~TYPE();
			}
		}
		HRESULT hr = SetSizeInternal( nNewMaxSize );

		if( nOldSize < nNewMaxSize )
		{
			assert( m_pData );
			if( m_pData )
			{
				for( int i = nOldSize; i < nNewMaxSize; ++i )
					::new ( &m_pData[i] ) TYPE;
			}
		}

		return hr;
	}
	template<typename TYPE> HRESULT raArray <TYPE>::Add( const TYPE& value )
	{
		HRESULT hr;
		if( FAILED( hr = SetSizeInternal( m_nSize + 1 ) ) )
			return hr;

		::new ( &m_pData[m_nSize] ) TYPE;

		m_pData[m_nSize] = value;
		++m_nSize;

		return S_OK;
	}
	template<typename TYPE> HRESULT raArray <TYPE>::Insert( raInt nIndex, const TYPE& value )
	{
		HRESULT hr;

		if( nIndex < 0 ||
			nIndex > m_nSize )
		{
			assert( false );
			return E_INVALIDARG;
		}

		if( FAILED( hr = SetSizeInternal( m_nSize + 1 ) ) )
			return hr;

		MoveMemory( &m_pData[nIndex + 1], &m_pData[nIndex], sizeof( TYPE ) * ( (int)m_nSize - (int)nIndex ) );

		::new ( &m_pData[nIndex] ) TYPE;

		m_pData[nIndex] = value;
		++m_nSize;

		return S_OK;
	}

	template<typename TYPE> HRESULT raArray <TYPE>::SetAt( raInt nIndex, const TYPE& value )
	{
		// Validate arguments
		if( nIndex < 0 ||
			nIndex >= m_nSize )
		{
			assert( false );
			return E_INVALIDARG;
		}

		m_pData[(int)nIndex] = value;
		return S_OK;
	}

	template<typename TYPE> raInt raArray <TYPE>::IndexOf( const TYPE& value, raInt iStart, raInt nNumElements )
	{
		// Validate arguments
		if( iStart < 0 ||
			iStart >= m_nSize ||
			nNumElements < 0 ||
			iStart + nNumElements > m_nSize )
		{
			assert( false );
			return -1;
		}

		// Search
		for( int i = iStart; i < ( iStart + nNumElements ); i++ )
		{
			if( value == m_pData[i] )
				return i;
		}

		// Not found
		return -1;
	}
	template<typename TYPE> raInt raArray <TYPE>::LastIndexOf( const TYPE& value, raInt iEnd, raInt nNumElements )
	{
		// Validate arguments
		if( iEnd < 0 ||
			iEnd >= m_nSize ||
			nNumElements < 0 ||
			iEnd - nNumElements < 0 )
		{
			assert( false );
			return -1;
		}

		// Search
		for( int i = iEnd; i > ( iEnd - nNumElements ); i-- )
		{
			if( value == m_pData[i] )
				return i;
		}

		// Not found
		return -1;
	}

	//--------------------------------------------------------------------------------------
	template<typename TYPE> HRESULT raArray <TYPE>::Remove( raInt nIndex )
	{
		if( nIndex < 0 ||
			nIndex >= m_nSize )
		{
			assert( false );
			return E_INVALIDARG;
		}

		// Destruct the element to be removed
		m_pData[(int)nIndex].~TYPE();

		// Compact the array and decrease the size
		MoveMemory( &m_pData[nIndex], &m_pData[(int)nIndex + 1], sizeof( TYPE ) * ( m_nSize - ( (int)(nIndex) + 1 ) ) );
		--m_nSize;

		return S_OK;
	}