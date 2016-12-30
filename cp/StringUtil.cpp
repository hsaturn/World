#include <StringUtil.hpp>

#include <map>
#include <iostream>
#include <vector>
#include <stdlib.h>

using namespace std;


bool StringUtil::mbInit=false;

void StringUtil::epure(string& sSrce)
{
	typedef map<unsigned char,string> CharConvert;
	typedef CharConvert::const_iterator const_iterator;
	static  CharConvert m;
	if (mbInit==false)
	{
		mbInit=true;
		for(unsigned char c=1; c<31;c++)
			m[c]=' ';
		m['_']=' ';
		m['(']=' ';
		m[')']=' ';
		m['A']='a';
		m['B']='b';
		m['C']='c';
		m['D']='d';
		m['E']='e';
		m['F']='f';
		m['G']='g';
		m['H']='h';
		m['I']='i';
		m['J']='j';
		m['K']='k';
		m['L']='l';
		m['M']='m';
		m['N']='n';
		m['O']='o';
		m['P']='p';
		m['Q']='q';
		m['R']='r';
		m['S']='s';
		m['T']='t';
		m['U']='u';
		m['V']='v';
		m['W']='w';
		m['X']='x';
		m['Y']='y';
		m['Z']='z';
		m['|']=' ';
		m[-18]='i';	// î
		m[-30]='a';	// â
		m[-55]='e';	// é
		m[-78]='2';	// ²
		m[-79]='%';	// +/-
		m[-22]='e'; // ê
		m[-23]='e';	// é
		m[-24]='e';	// è
		m[-25]='c';	// ç
		m[-30]='a';	// â
		m[-32]='a';	// à
		m[-52]='i'; // `I
		m[-67]='e'; // é
		m[-64]='a'; // `A
		m[-69]=' ';
		m[-75]=' ';
		m[-85]=' ';
		m[-96]=' ';
		m[-100]="oe";
		m[-103]=' ';
		m[-106]=' ';
		m[-108]=' ';
		m[-109]=' ';
		m[-110]=' ';
		m[-111]=' ';
		m['\'']=' ';
		m['(']=' ';
		m['~']=' ';
		m['{']=' ';
		m['}']=' ';
	}

	unsigned int i=0;
	//cout << sSrce << endl;
	while( i<sSrce.length())
	{
		// cout << ' ' << (int)sSrce[i]<< sSrce[i];
		if (sSrce[i]==-17 && sSrce[i+1]==-65)
		{
			sSrce.erase(i,2);
		}
		else
		{
			const_iterator oit=m.find((char)sSrce[i]);
			if (oit!=m.end())
			{
				sSrce.erase(i,1);
				sSrce.insert(i,oit->second);
				i+=oit->second.length();
			}
			else
				i++;
		}
	}
	//cout << sSrce << endl;
}

int StringUtil::LevenshteinDistance(const string &s1, const string &s2)
{
   string::size_type N1 = s1.length();
   string::size_type N2 = s2.length();
   string::size_type i, j;
   vector<int> T(N2+1);

   for ( i = 1; i <= N2; i++ )
      T[i] = i;

   for ( i = 0; i < N1; i++ )
   {
      T[0] = i+1;
      int corner = i;
      for ( j = 0; j < N2; j++ )
      {
         int upper = T[j+1];
         if ( s1[i] == s2[j] )
            T[j+1] = corner;
         else
            T[j+1] = min(T[j], min(upper, corner)) + 1;
         corner = upper;
      }
   }
   return T[N2];
}

void StringUtil::toLower(string& str) {
    for (unsigned int i=0;i<str.length();i++) 
        if (str[i] >= 0x41 && str[i] <= 0x5A) 
            str[i] = str[i] + 0x20;
}

void StringUtil::trim(string& s, bool bWithCr)
{
	while(s.length()>0 && (s[0]==' ' || (bWithCr && (s[0]==13 || s[0]==10))))
		s.erase(0,1);
}

long StringUtil::getLong(string& s)
{
	unsigned long i=0;
	long l=0;

	trim(s);
    
	while(s.length() && isdigit(s[i++]));
	if (isdigit(s[0]))
    {
     
		l=atol(s.substr(0,i).c_str());
        s.erase(0,i-1);
    }
    trim(s);
    return l;
}

string StringUtil::getWord(string& s, char cSep, char cSep2)
{
	string sWord;
	trim(s);
    if (s.length())
    {
		unsigned long i1=s.find(cSep);
        unsigned long i2=string::npos;
		if (i1==string::npos && i2==string::npos && s.length())
		{
			i1=s.length()+1;
		}

        if (cSep2!=0) i2=s.find(cSep2);
        if (i1!=string::npos && i2!=string::npos)
        {
			if (i1>i2) i1=i2;
        }
        else if (i1==string::npos)
         	i1=i2;
       	if (i1!=string::npos)
        {
        	sWord=s.substr(0,i1);
            s.erase(0,i1+1);
            trim(s);
        }
    }
    return sWord;
}

bool StringUtil::checkCharRemove(string& s, char c)
{
	if (s.length()>0 && s[0]==c)
    {
		s.erase(0,1);
        return true;
    }
    return false;
}

bool StringUtil::expectWord(string& s, const string& sWord)
{
	trim(s);
    if (s.substr(0,sWord.length())==sWord)
    {
     	s.erase(0,sWord.length());
        trim(s);
        return true;
    }
    return false;
}

string StringUtil::unserialize(string& s) throw(string)
{
 	trim(s);
   	string::size_type iSpc=s.find(' ');
    if (iSpc==string::npos)
        throw string("unable to unserialize [")+s.substr(0,20)+string("...]");
    unsigned long lSize=atol(s.substr(0,iSpc).c_str());
//     cout << "lsize " << lSize << endl;
    string sRet=s.substr(iSpc+1,lSize);
    s.erase(0,iSpc+lSize+1);
    return sRet;
}

template < class T >
long StringUtil::fillVector(T &container, string& s, char cSep)
{
	long lRet=0;
	string s2(s);
	while(s2.length())
	{
		string sWord=StringUtil::getWord(s,cSep);
		if (sWord.length())
		{
			lRet++;
			container.push_back(sWord);
		}
	}
	return lRet;
}

string StringUtil::getString(string& s)
{
	string sResult="";
	trim(s);
	if (s.length())
	{
		char cQuote=s[0];
		string::size_type iPos=s.find(cQuote,1);
		if (iPos!=string::npos)
		{
			sResult=s.substr(0,iPos);
			s.erase(0,iPos+1);
		}
	}
	return sResult;
}
