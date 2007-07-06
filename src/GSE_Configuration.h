/******************************************************************
 *   Copyright(c) 2006,2007 eNtity/Anssi Gröhn
 * 
 *   This file is part of GSE.
 *
 *   GSE is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *    GSE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with GSE; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 ******************************************************************/
#ifndef __GSE_Configuration_h__
#define __GSE_Configuration_h__
/////////////////////////////////////////////////////////////////
#include "GSE_graph.h"
#include "GSE_Vector.h"
#include <fstream>
#include <string>
#include <queue>
/////////////////////////////////////////////////////////////////
using std::string;
using std::fstream;
using std::queue;
using std::list;
using std::ifstream;
using std::ios;
/////////////////////////////////////////////////////////////////
// The configuration item types
/////////////////////////////////////////////////////////////////
typedef enum 
{

  CONFIGNODE_NOT_SPECIFIED = 0,
  CONFIGNODE_OPTION = 1

} GSE_ConfigItemType;
/////////////////////////////////////////////////////////////////
typedef GSE_Graph<GSE_ConfigItemType>		GSE_ConfigGraph;
typedef GSE_GraphNode<GSE_ConfigItemType>	GSE_ConfigGraphNode;
/////////////////////////////////////////////////////////////////
class GSE_ConfigOption : public GSE_ConfigGraphNode
{
 protected:
  string m_strValue;
 public:
  GSE_ConfigOption();
  void			SetValue( string &strValue );
  const string &	GetValue();
  void			SetValue( const char *szValue );
  void			SetValue( float fValue );
  void			SetValue( int iValue );
  float			GetFloat();
  int			GetInt();
  
};
/////////////////////////////////////////////////////////////////
typedef enum 
{

  CFG_RENDER_PASS_SEEK = 0,
  CFG_RENDER_PASS_DEBUG_PRINT 

} GSE_ConfigRenderPass_t;
/////////////////////////////////////////////////////////////////
class GSE_Configuration;
class GSE_ConfigRenderer : public GSE_MultiPassRenderer< GSE_ConfigRenderPass_t >
{
  friend class GSE_Configuration;
 protected:

  string m_strFoundValue;
  string m_strSeekOptionPath;
  string m_strIndent;

  GSE_ConfigOption *m_pConfigOption;

  queue<string> m_queueNodeNames;
  
 public:

  int Enter( GSE_GraphNode<GSE_ConfigItemType> *pNode );
  int Leave( GSE_GraphNode<GSE_ConfigItemType> *pNode );  
  int Handle_RenderPass_Seek_Enter( GSE_GraphNode<GSE_ConfigItemType> *pNode );
  int Handle_RenderPass_DebugPrint_Enter( GSE_GraphNode<GSE_ConfigItemType> *pNode );  
  GSE_ConfigRenderer();
  const string & GetFoundValue();
  
  void SetPath( string & strPath );
  void SetPath( const char *szPath );
  
};
/////////////////////////////////////////////////////////////////
#define GSE_CONFIGURATION_DEFAULT_SECTION_START_STRING "Section"
#define GSE_CONFIGURATION_DEFAULT_SECTION_END_STRING   "End"
/////////////////////////////////////////////////////////////////
class GSE_Configuration : public GSE_ConfigGraph
{
 protected:
  static const int MAX_LINE_LENGTH = 256;
  static const char COMMENT_CHAR   = '#';
  /// Are the warning messages shown. By default, yes.
  bool m_bShowWarnings;
  string m_strSectionString;
  string m_strSectionEndString;
  GSE_ConfigRenderer *m_pConfigRenderer;
  
 public:
  GSE_Configuration();
  GSE_Configuration( GSE_Configuration & ref);
  ~GSE_Configuration();
  /////////////////////////////////////////////////////////////////
  // NAME: ReadConfig
  // DESC: Reads configuration from file szFilename
  // PARAM:   const char szFilename - a zero-terminated 
  //	      string indicating the path to a config file.
  // RETURN : 
  //   0 on success,
  //   1 on invalid parameter
  //   2 when file couldn't be opened
  /////////////////////////////////////////////////////////////////
  int ReadConfig( const char *szFilename);
  // Prints the read config into Log output
  void PrintLog();
  // Clears read options to virgin state
  void Reset();
  // Returns value option of which path is strPath.
  // eg. "Screen|screen_width"
  string GetString( string strPath );
  float  GetFloat( string strPath );
  int	 GetInt( string strPath );
  long long GetLong( string strPath );
  char	 GetBoolean( string strPath );
  /////////////////////////////////////////////////////////////////
  template <size_t SIZE> GSE_Vector<float,SIZE> GetVector( string strPath )
  {
    size_t nFigStartPos = 0;
    size_t nCommaPos = 0;
    string strVect = GetString( strPath );
    GSE_Vector<float,SIZE>    vVector;
    TrimString(strVect);
    if ( strVect[0] == '(' && strVect[strVect.size()-1] == ')' )
    {
      for ( unsigned int nComp = 0; nComp<SIZE; nComp++)
      {
	nFigStartPos = nCommaPos+1;
	
	if ( nComp == SIZE - 1) 
	{
	  // The last char before ')'-sign.
	  nCommaPos = strVect.size() - 1;
	} 
	else 
	{ 
	  nCommaPos = strVect.find( ',', nFigStartPos );
	}
	
	if ( nCommaPos == string::npos )
	{
	  break;
	}
	vVector.m_pValues[nComp] = atof(strVect.substr( nFigStartPos, nCommaPos-nFigStartPos ).c_str());
      }
    }
    return vVector;
  }
  /////////////////////////////////////////////////////////////////
  // Returns a list of strings with the names of the suboptions.
  list<string> GetChildren( string strPath );
  
  void   SetSectionString( string strSectionString );
  void   SetSectionString( const char *szSectionString );
  void   SetSectionEndString( string strString );
  void   SetSectionEndString( const char *szString );
  /// Sets the flag for showing the warning messages.
  void   SetWarningsEnabled( bool bFlag );
 protected:
  string ReadLine( ifstream &fileStream );
  string &TrimString( string &str );
};
/////////////////////////////////////////////////////////////////
// $Id: GSE_Configuration.h,v 1.13 2007/03/20 12:19:49 entity Exp $
// desc : Header for GSE_Configuration. Reads configuration options
//	  from file.
// author : eNtity
// history : 
//		$Log: GSE_Configuration.h,v $
//		Revision 1.13  2007/03/20 12:19:49  entity
//		more
//
//		Revision 1.12  2007/03/20 12:19:21  entity
//		code cleanups
//
//		Revision 1.11  2007/03/20 10:44:13  entity
//		removed references to GraphNodeTraveller
//
//		Revision 1.10  2007/03/09 12:25:27  entity
//		Vector parser function template added.
//
//		Revision 1.9  2007/03/05 11:30:22  entity
//		Uses now GSE_GraphNodeTraveler
//
//		Revision 1.8  2007/03/02 07:24:43  entity
//		changed method name
//
//		Revision 1.7  2007/03/02 07:22:47  entity
//		Added support for suppressing the warnings
//
//		Revision 1.6  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.5  2006/11/20 19:46:56  entity
//		Comments
//
//		Revision 1.4  2006/10/20 16:07:21  entity
//		added changeable Section start and end strings + getBoolean()
//
//		Revision 1.3  2006/10/14 22:03:04  entity
//		First fully operational version
//
//		Revision 1.2  2006/10/14 19:02:24  entity
//		First working version
//
//		Revision 1.1  2006/10/14 16:23:43  entity
//		Initial import to CVS
//
/////////////////////////////////////////////////////////////////
#endif
