#pragma once

// m3u Datein
class raPlayerPlayliste : public IPlayliste
{
public:
	raPlayerPlayliste(void);
	~raPlayerPlayliste(void);

	virtual bool Open(raString FileName);	// überschreibung der Methode aus IPlayliste
	virtual bool Save(raString Filename);	// überschreibung der Methode aus IPlayliste
};


// m3u
