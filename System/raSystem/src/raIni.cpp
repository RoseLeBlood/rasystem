#include "..\include\raMain.h"

namespace System
{
	bool raIni::Open(const char filename[])
	{
		if(!Util::raFile::raFileExists(filename))
			return false;

		// init some variables
		this->Closed = false;
		this->withoutSections = true;
		this->sectionnames.push_back(INI_DEFAULT_SECTION);
		this->buffer.push_back( std::vector<raString>() );
		this->filename = filename;
		this->sections = 0;
		this->entries = 0;

		//open the file
		std::ifstream inifile(filename);
		raString iniline("");
		int section_idx = 0; // counts the section, needed for the buffer vector
		raString section_tmp_name;
		size_t pos1;
		size_t pos2;

		if(inifile.is_open())
		{
			this->InitGood = true;
			while(!inifile.eof())
			{
				// Read section and insert them to the vector
				getline(inifile, iniline);
				if(iniline.length() > INI_MAX_LENGTH) // too large, avoid buffer overflows
				iniline = iniline.substr(0, INI_MAX_LENGTH);

				if(iniline[0] == '[' && iniline.find(']') != raString::npos) // a section starts
				{
					// change mode
					if(this->withoutSections)
						this->withoutSections = false;
					// statistics
					this->sections++;

					// Remove [ and ]
					pos1 = iniline.find("[");
					pos2 = iniline.find("]", pos1+1);
					section_tmp_name = iniline.substr(pos1+1, pos2-1);

					// Overwrite default section
					if(section_idx == 0)
						this->sectionnames[0] = section_tmp_name;
					else
						this->sectionnames.push_back(section_tmp_name);

					this->buffer.push_back( std::vector<raString>() ); // create a new dimension

					section_idx++;

					// Reset pos1 and pos2
					pos1 = 0;
					pos2 = 0;
				}
				else
				{
					// statistics
					if(iniline[0] != ';' && iniline.find('=') != raString::npos)
					this->entries++;

					if(section_idx > 0)
						this->buffer[section_idx-1].push_back(iniline);
					else
						this->buffer[0].push_back(iniline);
				}
			}
			m_isloaded = true;
			ROK("Config [ini] geladen");
		}
		else
			this->InitGood = false;

		inifile.close();
		return InitGood;
	}
	void raIni::Clear()
	{
		// clear buffers
		this->sectionnames.clear();
		this->buffer.clear();

		//statistics
		this->entries = 0;
		this->sections = 0;
	}
	void raIni::Close()
	{
		int emptylines = 0;
		std::fstream inifile;

		inifile.open(this->filename.c_str(), std::ios::out);
		if(inifile.is_open())
		{
			this->InitGood = false; // file is closed, so false
			std::vector<std::vector <raString> >::iterator section_it;
			std::vector<raString>::iterator sectionnames_it;
			std::vector<raString>::iterator line_it;

			for(section_it = this->buffer.begin(), sectionnames_it = this->sectionnames.begin(); section_it != this->buffer.end(); ++section_it, ++sectionnames_it)
			{
				if(sectionnames_it != this->sectionnames.end())
				{
					if(!this->withoutSections) // only write the section if the section name isn't empty and if the mode "without Section" isn't true
						inifile << '[' << *sectionnames_it << ']' << std::endl;
				}

				// get all lines of the section
				for(line_it = section_it->begin(); line_it != section_it->end(); ++line_it)
				{
					// don't add a new line if the section and entry is the last one
					if((this->buffer.end()-section_it) == 2 && (section_it->end() - line_it) == 2) // = last entry of the last section
					{
						if(line_it->compare(INI_LINE_DONTSAVE)) // only write the line if it isn't marked as removed
							inifile << *line_it;
					}
					else
					{
						if(line_it->compare(INI_LINE_DONTSAVE)) // only write the line if it isn't marked as removed
							inifile << *line_it << std::endl;
					}
				}
			}
			inifile.close();
			this->Closed = true;
		}
		else
		{
			this->InitGood = false;
			this->Closed = true;
		}
	}
	bool raIni::Good()
	{
		bool returnVal = this->InitGood;
		return returnVal;
	}

	int raIni::GetInt(const char section[], const char key[])
	{
		// Get the right dimension for accessing the buffer
		std::vector<raString>::iterator sectionnames_it;
		std::vector<raString>::iterator buffer_it;
		std::vector<raString>::iterator buffer_end;
		bool lsection_found = false;
		int lsection = 0;

		raString lkey;
		raString lvalue;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
		{
			if(!(*sectionnames_it).compare(section))
			{
				lsection_found = true;
				break;
			}
			lsection++;
		}

		if(!lsection_found  && !this->withoutSections) return -1;

		buffer_end = this->buffer[lsection].end();
		for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
		{
			lkey = this->getKeyFromString(*buffer_it);

			if(!lkey.compare(key))
			{
				lvalue = this->getValueFromString(*buffer_it);
				return (this->stringtonum<int>(lvalue));
			}
		}

		// key not found
		return -1;
	}
	long raIni::GetLong(const char section[], const char key[])
	{
		 // Get the right dimension for accessing the buffer
		std::vector<raString>::iterator sectionnames_it;
		std::vector<raString>::iterator buffer_it;
		std::vector<raString>::iterator buffer_end;

		bool lsection_found = false;
		int lsection = 0;

		raString lkey;
		raString lvalue;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
		{
			if(!(*sectionnames_it).compare(section))
			{
				lsection_found = true;
				break;
			}
		lsection++;
		}

		if(!lsection_found  && !this->withoutSections) return -1;

		buffer_end = this->buffer[lsection].end();
		for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
		{
			lkey = this->getKeyFromString(*buffer_it);
			if(!lkey.compare(key))
			{
				lvalue = this->getValueFromString(*buffer_it);
				return (this->stringtonum<long>(lvalue));
			}
		}
		return -1;
	}
	bool raIni::GetBool(const char section[], const char key[])
	{
		// Get the right dimension for accessing the buffer
		std::vector<raString>::iterator sectionnames_it;
		std::vector<raString>::iterator buffer_it;
		std::vector<raString>::iterator buffer_end;

		bool lsection_found = false;
		int lsection = 0;

		raString lkey;
		raString lvalue;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
		{
			if(!(*sectionnames_it).compare(section))
			{
				lsection_found = true;
				break;
			}
			lsection++;
		}

		if(!lsection_found  && !this->withoutSections) return false;

		buffer_end = this->buffer[lsection].end();
		for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
		{
			lkey = this->getKeyFromString(*buffer_it);

			if(!lkey.compare(key))
			{
				int lretval;
				lvalue = this->getValueFromString(*buffer_it);
				lretval = (this->stringtonum<int>(lvalue));

				if(lretval == 1)
					return true;
				else
					return false;
			}
		}
		return false;
	}
	raString raIni::GetString(const char section[], const char key[])
	{
		// Get the right dimension for accessing the buffer
		std::vector<raString>::iterator sectionnames_it;
		std::vector<raString>::iterator buffer_it;
		std::vector<raString>::iterator buffer_end;

		bool lsection_found = false;
		int lsection = 0;

		raString lkey;
		raString lvalue;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); sectionnames_it++)
		{
				if(!(*sectionnames_it).compare(section))
				{
					lsection_found = true;
					break;
				}
			lsection++;
		}

		if(!lsection_found  && !this->withoutSections) return "";

		buffer_end=this->buffer[lsection].end();
		for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
		{
			lkey = this->getKeyFromString(*buffer_it);

			if(!lkey.compare(key))
			{
				raString lstring = getValueFromString(*buffer_it);
				return lstring;
			}
		}

		// key not found
		return "";
	}
	raString raIni::Get(const char section[], const char key[])
	{
		return this->GetString(section, key);
	}

	void raIni::SetInt(const char section[],const char key[], int newval)
	{
		// Get the right dimension for accessing the buffer
		std::vector<std::string>::iterator sectionnames_it;
		std::vector<std::string>::iterator buffer_it;
		std::vector<std::string>::iterator buffer_end;

		bool lsection_found = false;
		bool lkey_found = false;
		int lsection = 0;
		int lkeyline = 0;

		std::string lkey;

		if(!this->withoutSections)
		{
			for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
			{
				if(!(*sectionnames_it).compare(section))
				{
					lsection_found = true;
					break;
				}
				lsection++;
			}
		}

		if(!lsection_found  && !this->withoutSections)
			this->CreateSection(section);
		else
		{
			buffer_end = this->buffer[lsection].end();
			for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
			{
				lkey = this->getKeyFromString(*buffer_it);

				if(!lkey.compare(key))
				{
					*buffer_it = lkey+'=';
					*buffer_it += newval;
					lkey_found = true;
					break;
				}
				lkeyline++;
			}
		}

		if(!lkey_found)
		{
			// statistics
			this->entries++;

			std::string lnewentry;
			lnewentry.append(key);
			lnewentry.append("=");
			lnewentry += newval;
			this->buffer[lsection].push_back(lnewentry);
		}
	}
	void raIni::SetLong(const char section[], const char key[], long newval)
	{
		// Get the right dimension for accessing the buffer
		std::vector<std::string>::iterator sectionnames_it;
		std::vector<std::string>::iterator buffer_it;
		std::vector<std::string>::iterator buffer_end;

		bool lsection_found = false;
		bool lkey_found = false;
		int lsection = 0;
		int lkeyline = 0;

		std::string lkey;

		if(!this->withoutSections)
		{
			for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
			{
				if(!(*sectionnames_it).compare(section))
				{
					lsection_found = true;
					break;
				}
				lsection++;
			}
		}

		if(!lsection_found  && !this->withoutSections)
			this->CreateSection(section);
		else
		{
			buffer_end = this->buffer[lsection].end();
			for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
			{
				lkey = this->getKeyFromString(*buffer_it);

				if(!lkey.compare(key))
				{
					*buffer_it = lkey+'=';
					*buffer_it += newval;
					lkey_found = true;
					break;
				}
				lkeyline++;
			}
		}

		if(!lkey_found)
		{
			// statistics
			this->entries++;

			std::string lnewentry;
			lnewentry.append(key);
			lnewentry.append("=");
			lnewentry += newval;
			this->buffer[lsection].push_back(lnewentry);
		}
	}
	void raIni::SetBool(const char section[], const char key[], bool newval)
	{
		// Get the right dimension for accessing the buffer
		std::vector<std::string>::iterator sectionnames_it;
		std::vector<std::string>::iterator buffer_it;
		std::vector<std::string>::iterator buffer_end;

		bool lsection_found = false;
		bool lkey_found = false;
		int lsection = 0;
		int lkeyline = 0;

		std::string lkey;

		if(!this->withoutSections)
		{
			for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
			{
				if(!(*sectionnames_it).compare(section))
				{
					lsection_found = true;
					break;
				}
				lsection++;
			}
		}

		if(!lsection_found  && !this->withoutSections)
			this->CreateSection(section);
		else
		{
			buffer_end = this->buffer[lsection].end();
			for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
			{
				lkey = this->getKeyFromString(*buffer_it);

				if(!lkey.compare(key))
				{
					*buffer_it = lkey+'=';
					*buffer_it += newval;
					lkey_found = true;
					break;
				}
				lkeyline++;
			}
		}

		if(!lkey_found)
		{
			// statistics
			this->entries++;

			std::string lnewentry;
			lnewentry.append(key);
			lnewentry.append("=");
			lnewentry += newval;
			this->buffer[lsection].push_back(lnewentry);
		}
	}
	void raIni::SetString(const char section[], const char key[], raString newval)
	{
		// Get the right dimension for accessing the buffer
		std::vector<std::string>::iterator sectionnames_it;
		std::vector<std::string>::iterator buffer_it;
		std::vector<std::string>::iterator buffer_end;

		bool lsection_found = false;
		bool lkey_found = false;
		int lsection = 0;
		int lkeyline = 0;

		std::string lkey;

		if(!this->withoutSections)
		{
			for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
			{
				if(!(*sectionnames_it).compare(section))
					{
					lsection_found = true;
					break;
					}
				lsection++;
			}
		}

		if(!lsection_found  && !this->withoutSections)
			this->CreateSection(section);
		else
		{
			buffer_end = this->buffer[lsection].end();
			for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
			{
				lkey = this->getKeyFromString(*buffer_it);

				if(!lkey.compare(key))
				{
					*buffer_it = lkey+'=';
					*buffer_it += newval;
					lkey_found = true;
					break;
				}
				lkeyline++;
			}
		}

		if(!lkey_found)
		{
			// statistics
			this->entries++;

			std::string lnewentry;
			lnewentry.append(key);
			lnewentry.append("=");
			lnewentry+=newval;
			this->buffer[lsection].push_back(lnewentry);
		}
	}
	void raIni::Set(const char section[], const char key[], const char newval[])
	{
		// Get the right dimension for accessing the buffer
		std::vector<raString>::iterator sectionnames_it;
		std::vector<raString>::iterator buffer_it;
		std::vector<raString>::iterator buffer_end;

		bool lsection_found = false;
		bool lkey_found = false;
		int lsection = 0;
		int lkeyline = 0;

		raString lkey;

		if(!this->withoutSections)
		{
			for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
			{
				if(!(*sectionnames_it).compare(section))
				{
					lsection_found = true;
					break;
				}
				lsection++;
			}
		}

		if(!lsection_found && !this->withoutSections)
			this->CreateSection(section);
		else
		{
			buffer_end = this->buffer[lsection].end();
			for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
			{
				lkey = this->getKeyFromString(*buffer_it);

				if(!lkey.compare(key))
				{
					*buffer_it = lkey+'=';
					*buffer_it += newval;
					lkey_found = true;
					break;
				}
				lkeyline++;
			}
		}

		if(!lkey_found)
		{
			// statistics
			this->entries++;

			raString lnewentry;
			lnewentry.append(key);
			lnewentry.append("=");
			lnewentry += newval;
			this->buffer[lsection].push_back(lnewentry);
		}
	}

	void raIni::CreateSection(const char section[])
	{
		bool lsecfound = false;
		std::vector<std::string>::iterator sectionnames_it;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); sectionnames_it++)
		{
			if(!(*sectionnames_it).compare(section))
				lsecfound = true;
		}

		if(!lsecfound)
		{
			this->sectionnames.push_back(section);
			this->buffer.push_back( std::vector<std::string>() );
			this->sections++;
		}
	}
	void raIni::RemoveSection(const char section[])
	{
		std::vector<std::string>::iterator sectionnames_it;
		int lpos = 0;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); sectionnames_it++)
		{
			if(!(*sectionnames_it).compare(section))
			{
				this->buffer.erase(this->buffer.begin()+lpos);
				sectionnames_it->clear();

				 // statistics
				this->sections--;
				break;
			}
			lpos++;
		}
	}
	void raIni::RemoveEntry(const char section[], const char key[])
	{
		// Get the right dimension for accessing the buffer
		std::vector<std::string>::iterator sectionnames_it;
		std::vector<std::string>::iterator buffer_it;
		std::vector<std::string>::iterator buffer_end;
		bool lsection_found = false;
		int lsection = 0;

		std::string lkey;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
		{
			if(!(*sectionnames_it).compare(section))
			{
				lsection_found = true;
				break;
			}
			lsection++;
		}

		if(lsection_found)
		{
			buffer_end = this->buffer[lsection].end();
			for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
			{
				lkey = this->getKeyFromString(*buffer_it);

				if(!lkey.compare(key))
				{
					// statistics
					this->entries--;

					*buffer_it = INI_LINE_DONTSAVE;
					break;
				}
			}
		}
	}
	bool raIni::ValidSection(const char section[])
	{
		std::vector<std::string>::iterator sectionnames_it;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
		{
			if(!sectionnames_it->compare(section))
				return true;
		}
		return false;
	}
	bool raIni::ValidEntry(const char section[], const char key[])
	{
		// Get the right dimension for accessing the buffer
		std::vector<std::string>::iterator sectionnames_it;
		std::vector<std::string>::iterator buffer_it;
		std::vector<std::string>::iterator buffer_end;

		bool lsection_found = false;
		int lsection = 0;

		std::string lkey;

		for(sectionnames_it = this->sectionnames.begin(); sectionnames_it < this->sectionnames.end(); ++sectionnames_it)
		{
			if(!(*sectionnames_it).compare(section))
			{
				lsection_found = true;
				break;
			}
			lsection++;
		}

		if(!lsection_found)
			return false;
		else
		{
			buffer_end = this->buffer[lsection].end();
			for(buffer_it = this->buffer[lsection].begin(); buffer_it < buffer_end; ++buffer_it)
			{
				lkey = this->getKeyFromString(*buffer_it);
				if(!lkey.compare(key))
					return true;
			}
		}
		return false;
	}
	int raIni::CountSections()
	{
		int lsections = this->sections;
		return lsections;
	}
	int raIni::CountEntries()
	{
		int lentries = this->entries;
		return lentries;
	}
	std::string raIni::getKeyFromString(std::string mystring)
	{
		size_t i = mystring.find('=');
		if(i>=0)
		{
			return mystring.substr(0, i);
		}
		else return "";
	}

	/*
	Name: getValueFromString
	Function: Gets a value from a string, which is in the ini format
	(key = value)
	returns: The value as std:string
	*/
	std::string raIni::getValueFromString(std::string mystring)
	{
		size_t i = mystring.find_last_of('=');
		if(i>=0)
		{
			return mystring.substr(i+1);
		}
		else return "";
	}

	template <class T>
	std::string raIni::numtostring(T num)
	{
		std::string mystring;
		std::ostringstream convert;
		convert << num;
		mystring = convert.str();
		return mystring;
	}

	template <class T>
	T raIni::stringtonum(std::string mystring)
	{
		T num;
		std::istringstream convert(mystring);
		//convert.str(mystring);

		if( !(convert >> num) ) num = 0;

		return num;
	}
}