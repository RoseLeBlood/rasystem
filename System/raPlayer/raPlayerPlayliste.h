#pragma once

// m3u Datein
class raPlayerPlayliste : public IPlayliste
{
public:
	raPlayerPlayliste(void);
	~raPlayerPlayliste(void);

	virtual bool Open(raString FileName);	// �berschreibung der Methode aus IPlayliste
	virtual bool Save(raString Filename);	// �berschreibung der Methode aus IPlayliste
};


// m3u
