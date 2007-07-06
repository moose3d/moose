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
/////////////////////////////////////////////////////////////////
#include "GSE_Configuration.h"
#include "GSE_Logger.h"
/////////////////////////////////////////////////////////////////
#include <fstream>
#include <stack>
/////////////////////////////////////////////////////////////////
using std::fstream;
using std::stack;
/////////////////////////////////////////////////////////////////
GSE_Configuration::GSE_Configuration()
{
  m_pConfigRenderer = new GSE_ConfigRenderer();
  m_strSectionString = GSE_CONFIGURATION_DEFAULT_SECTION_START_STRING;
  m_strSectionEndString = GSE_CONFIGURATION_DEFAULT_SECTION_END_STRING;
  m_bShowWarnings = true;
}
/////////////////////////////////////////////////////////////////
GSE_Configuration::~GSE_Configuration()
{
  delete m_pConfigRenderer;
}
/////////////////////////////////////////////////////////////////
GSE_Configuration::GSE_Configuration( GSE_Configuration & ref )
{
  m_pConfigRenderer = new GSE_ConfigRenderer( *(ref.m_pConfigRenderer ));
}
/////////////////////////////////////////////////////////////////
int
GSE_Configuration::ReadConfig( const char *szFilename )
{

  int iRetval = 0;
  // Cleanup previous config stuctures, if any
  Reset();
  stack<GSE_ConfigOption *> stackSections;

  GSE_ConfigOption *pConfigRoot = CreateNode<GSE_ConfigOption>();
  pConfigRoot->SetName( "ConfigRoot" ); 
  stackSections.push( pConfigRoot );
  
  
  if ( szFilename == NULL )
  {
    GSE_ERR( __FUNCTION__ << "Filename is NULL" );
    iRetval = 1;
    return iRetval;
  } 
  
  ifstream fileIn;
  fileIn.open( szFilename, ios::in );

  if ( !fileIn )
  {
    GSE_ERR( "Couldn't open file " << szFilename );
    iRetval = 2;
  }
  else
  {
    string strLine;
    unsigned int nLineNum = 0;
    while( fileIn )
    {
      strLine = ReadLine( fileIn );
      TrimString(strLine);
      nLineNum++;

      
      // Skip lines beginning with COMMENT_CHAR
      if ( strLine.c_str()[0] == COMMENT_CHAR ) continue;
      // Skip empty lines 
      if ( strLine.length() == 0 ) continue;


      if ( strLine.substr(0,m_strSectionString.length()+1).compare(m_strSectionString + " ") == 0)
      {
	// The beginning of a section
	string strSectionName = strLine.substr( m_strSectionString.length()+1, 
						strLine.length()-m_strSectionString.length()+1 );
	TrimString(strSectionName);
	GSE_DEBUG(m_strSectionString << " '" <<  strSectionName << "' found.");
	
	GSE_ConfigOption *pConfigSection = CreateNode<GSE_ConfigOption>();
	pConfigSection->SetName( (char *)strSectionName.c_str());	


	stackSections.top()->AddEdge( pConfigSection );
	stackSections.push( pConfigSection );
      } 
      else if ( strLine.compare(m_strSectionEndString) == 0 )
      {
	// The End of a section 
	GSE_DEBUG(m_strSectionEndString << " of a " << m_strSectionString);
 	stackSections.pop();
      } 
      else
      {
	// The parameter = value pair
	size_t delimPos = strLine.find( "=", 0 );
	
	if ( delimPos == string::npos )
	{
	  if ( m_bShowWarnings )
	  {
	    GSE_WARN( szFilename 
		      << ", line " << nLineNum 
		      << ": Not a parameter line, skipping."
		      << "'" <<strLine << "'");	  
	  }
	} 
	else if ( stackSections.size() > 1 ) // more than root node is required.
	{
	  string strParamName  = strLine.substr(0, delimPos-1 );
	  string strParamValue = strLine.substr(delimPos+1, strLine.length()-delimPos);
	  
	  TrimString( strParamName );
	  TrimString( strParamValue );
	  // Create config option node
	  GSE_ConfigOption *pConfigOption = CreateNode<GSE_ConfigOption>();
	  pConfigOption->SetName( strParamName.c_str());
	  pConfigOption->SetValue( strParamValue );
	  
	  
	  stackSections.top()->AddEdge( pConfigOption );
	}
      }
    }
    // Remove also the root section
    stackSections.pop();

    if ( !stackSections.empty())
    {
      if ( m_bShowWarnings )
      {
	GSE_WARN( "Configuration file " 
		  << szFilename 
		  << " might not be entirely valid. " );
      }
    }
    fileIn.close();
  } 
  return iRetval;
}
/////////////////////////////////////////////////////////////////
string
GSE_Configuration::ReadLine( ifstream &fileStream )
{
  string strLine;
  char buf[MAX_LINE_LENGTH+1];
  int nNumReadChars = 0;
  char bNewLineFound = 0;
  char tmpChar;
  memset( buf, '\0', MAX_LINE_LENGTH+1);
  
  while( nNumReadChars < MAX_LINE_LENGTH && 
	 !bNewLineFound && 
	 !fileStream.eof() )
  {
    fileStream.get( tmpChar );    

    if( fileStream.eof() ) continue;
    if ( tmpChar == '\n' ) bNewLineFound = 1;
    else
    {
      buf[nNumReadChars] = tmpChar;
      nNumReadChars++;
    }
  }
  if ( !bNewLineFound )
  {
    // Fforward to the end of line (or to EOF )
    do {
      fileStream.get( tmpChar );
    } while ( tmpChar != '\n' && !fileStream.eof() );
    
  }
  buf[nNumReadChars] = '\0';
  strLine = buf;
  return strLine;
}
/////////////////////////////////////////////////////////////////
string &
GSE_Configuration::TrimString( string &str )
{
  if ( str.length() > 0 )
  {
    const char *pStr = str.c_str();
    size_t pos;
    char bHasWhiteSpaceInEnd   = (pStr[str.length()-1] == ' ' || 
				  pStr[str.length()-1] == '\t' );
    char bHasWhiteSpaceInStart = (pStr[0] == ' '||pStr[0]=='\t');
    
    
    if ( bHasWhiteSpaceInStart )
    {
      if ( (pos = str.find_first_not_of(" \t")) != string::npos )
      {
	str.erase(0,pos);
      }
      else
	str.erase();
    }
    
    if ( bHasWhiteSpaceInEnd  )
    {
      if ( (pos = str.find_last_not_of(" \t")) != string::npos )
      {
	str.erase(pos+1,str.length()-1);
      }
      else
	str.erase();
    }
  }
  return str;
}
/////////////////////////////////////////////////////////////////
void
GSE_Configuration::Reset()
{
  RemoveEdges();
  RemoveNodes();
}
/////////////////////////////////////////////////////////////////
string 
GSE_Configuration::GetString( string strPath )
{
  string strValue = "";
  m_pConfigRenderer->SetRenderPass( CFG_RENDER_PASS_SEEK );
  m_pConfigRenderer->SetPath( strPath );
  GSE_ConfigOption *pOption = 
    (GSE_ConfigOption *)SeekNodeByNameAndType( "ConfigRoot", 
					     CONFIGNODE_OPTION);

  if ( pOption != NULL )
  {
    //pOption->Render( m_pConfigRenderer );
    Graph::TravelDF<GSE_ConfigItemType,GSE_ConfigRenderer>( pOption, m_pConfigRenderer);
    strValue = m_pConfigRenderer->GetFoundValue();
  }
  return strValue;
}
/////////////////////////////////////////////////////////////////
float
GSE_Configuration::GetFloat( string strPath )
{
  string strValue = GetString(strPath);
  return atof(strValue.c_str());
}
/////////////////////////////////////////////////////////////////
int
GSE_Configuration::GetInt( string strPath )
{
  string strValue = GetString(strPath);
  return atoi(strValue.c_str());
}
/////////////////////////////////////////////////////////////////
long long
GSE_Configuration::GetLong( string strPath )
{
  string strValue = GetString(strPath);
  return atoll(strValue.c_str());
}
/////////////////////////////////////////////////////////////////
char	 
GSE_Configuration::GetBoolean( string strPath )
{
  string value = GetString( strPath );
  
  return ( !value.compare("true") || 
	   !value.compare("yes") ||
	   !value.compare("1") );
  
}
/////////////////////////////////////////////////////////////////
void 
GSE_Configuration::PrintLog()
{
  m_pConfigRenderer->SetRenderPass( CFG_RENDER_PASS_DEBUG_PRINT );
  GSE_ConfigOption *pOption =
      (GSE_ConfigOption *)SeekNodeByNameAndType( "ConfigRoot", 
						 CONFIGNODE_OPTION);
  if ( pOption != NULL )
  {
    //pOption->Render( m_pConfigRenderer );
    //GSE_ConfigGraph::TravelDF( pOption, m_pConfigRenderer);
    Graph::TravelDF<GSE_ConfigItemType,GSE_ConfigRenderer>( pOption, m_pConfigRenderer);
  }
}
/////////////////////////////////////////////////////////////////
void   
GSE_Configuration::SetSectionString( const char *szSectionString )
{
  if ( szSectionString != NULL )
    m_strSectionString = szSectionString;
  else
    m_strSectionString.erase();
}
/////////////////////////////////////////////////////////////////
void   
GSE_Configuration::SetSectionString( string strSectionString )
{
  m_strSectionString = strSectionString;
}
/////////////////////////////////////////////////////////////////
void
GSE_Configuration::SetSectionEndString( string strString )
{
  m_strSectionEndString = strString;
}
/////////////////////////////////////////////////////////////////
void
GSE_Configuration::SetSectionEndString( const char *szString )
{
  if ( szString != NULL )
    m_strSectionEndString = szString;
  else
    m_strSectionEndString.erase();
}
/////////////////////////////////////////////////////////////////
list<string> 
GSE_Configuration::GetChildren( string strPath )
{
  list<string> lstNodes;
  m_pConfigRenderer->SetRenderPass( CFG_RENDER_PASS_SEEK );
  m_pConfigRenderer->SetPath( strPath );
  GSE_ConfigOption *pOption = 
    (GSE_ConfigOption *)SeekNodeByNameAndType( "ConfigRoot", 
					     CONFIGNODE_OPTION);

  if ( pOption != NULL )
  {

    m_pConfigRenderer->m_pConfigOption = NULL;
    
    //pOption->Render( m_pConfigRenderer );
    //GSE_ConfigGraph::TravelDF( pOption, m_pConfigRenderer);
    Graph::TravelDF<GSE_ConfigItemType,GSE_ConfigRenderer>( pOption, m_pConfigRenderer);
    if ( m_pConfigRenderer->m_pConfigOption != NULL )
    {
      list< GSE_GraphEdge<GSE_ConfigItemType> *>::iterator it = m_pConfigRenderer->m_pConfigOption->GetLeavingEdges().begin();
      for( ; it != m_pConfigRenderer->m_pConfigOption->GetLeavingEdges().end(); it++)
      {
	lstNodes.push_back( string( (*it)->GetToNode()->GetName() ) );
      }
      
    }
  }
  return lstNodes;
}
/////////////////////////////////////////////////////////////////
GSE_ConfigOption::GSE_ConfigOption()
{
  SetValue("");
  SetType(CONFIGNODE_OPTION);
}
/////////////////////////////////////////////////////////////////
void 
GSE_ConfigOption::SetValue( string &strValue )
{
  m_strValue = strValue;
}
/////////////////////////////////////////////////////////////////
const string & 
GSE_ConfigOption::GetValue()
{
  return m_strValue;
}
/////////////////////////////////////////////////////////////////
void 
GSE_ConfigOption::SetValue( const char *szValue )
{
  m_strValue = szValue;
}
/////////////////////////////////////////////////////////////////
void 
GSE_ConfigOption::SetValue( float fValue )
{
  char szTmpBuf[1024];
  snprintf( szTmpBuf, 1024, "%f.9f", fValue );
  m_strValue = szTmpBuf;
}
/////////////////////////////////////////////////////////////////
void 
GSE_ConfigOption::SetValue( int iValue )
{
  char szTmpBuf[1024];
  snprintf( szTmpBuf, 1024, "%d", iValue );
  m_strValue = szTmpBuf;
}
/////////////////////////////////////////////////////////////////
float 
GSE_ConfigOption::GetFloat()
{
  return atof(m_strValue.c_str());
}
/////////////////////////////////////////////////////////////////
int 
GSE_ConfigOption::GetInt()
{
  return atoi(m_strValue.c_str());
}
/////////////////////////////////////////////////////////////////
GSE_ConfigRenderer::GSE_ConfigRenderer()
{
  SetPath("");
  m_pConfigOption = NULL;
}
/////////////////////////////////////////////////////////////////
int 
GSE_ConfigRenderer::Enter( GSE_GraphNode<GSE_ConfigItemType> *pNode )
{
  int iCulled = 0;
  switch( GetRenderPass() )
  {
  case CFG_RENDER_PASS_SEEK:
    iCulled = Handle_RenderPass_Seek_Enter( pNode );
    break;
  case CFG_RENDER_PASS_DEBUG_PRINT:
    iCulled = Handle_RenderPass_DebugPrint_Enter( pNode );
    break;
  }
  return iCulled;
}
/////////////////////////////////////////////////////////////////
int
GSE_ConfigRenderer::Leave( GSE_GraphNode<GSE_ConfigItemType> *pNode )
{
  int iRetval = 0;
  GSE_ConfigOption *pConfig = (GSE_ConfigOption *)pNode;

  if ( GetRenderPass() == CFG_RENDER_PASS_DEBUG_PRINT )
  {
    if ( pConfig->HasLeavingEdges())
    {
      m_strIndent.erase(0,1);
      GSE_LOG(m_strIndent << "End of section");
    }
  }
  return iRetval;
}
/////////////////////////////////////////////////////////////////
const string & 
GSE_ConfigRenderer::GetFoundValue()
{
  return m_strFoundValue;  
}
/////////////////////////////////////////////////////////////////
void 
GSE_ConfigRenderer::SetPath( string & strPath )
{
  
  SetPath( strPath.c_str() );
}
/////////////////////////////////////////////////////////////////
void 
GSE_ConfigRenderer::SetPath( const char *szPath )
{
  m_strSeekOptionPath = szPath;
  string tmpStr = m_strSeekOptionPath;
  

  // Clear previous queue
  while( !m_queueNodeNames.empty() ) 
  { 
    m_queueNodeNames.pop();
  }

  
  // Clear previous value 
  m_strFoundValue.erase();  
  m_queueNodeNames.push( "ConfigRoot" );

  while( tmpStr.length() > 0)
  {

    
    size_t pos = tmpStr.find("|",0);
    
    if ( pos == string::npos )
    {
      m_queueNodeNames.push( tmpStr );
      tmpStr.erase();
    }
    else
    {
      m_queueNodeNames.push( tmpStr.substr(0, pos ) );
      tmpStr.erase(0, pos+1 );
    }
  }

}
/////////////////////////////////////////////////////////////////
int 
GSE_ConfigRenderer::Handle_RenderPass_Seek_Enter( GSE_GraphNode<GSE_ConfigItemType> *pNode )
{

  int iCulled = 0;
  GSE_ConfigOption *pConfig = (GSE_ConfigOption *)pNode;
  switch( pNode->GetType())
  {
  case CONFIGNODE_NOT_SPECIFIED:
    GSE_WARN( "GSE_ConfigRenderer : Undefined node!" );
    iCulled = 1;
    break;
  case CONFIGNODE_OPTION:

    if ( m_queueNodeNames.size() == 0 ) 
    {
      iCulled = 1;
      break;
    }
    iCulled = (m_queueNodeNames.front().compare(pNode->GetName()) != 0);
    if ( iCulled == 0)
    {
      m_queueNodeNames.pop();
      if ( m_queueNodeNames.empty())
      {
	m_strFoundValue = pConfig->GetValue();
	m_pConfigOption = pConfig;
	iCulled = 1;
      }
    }
    break;
  }

  return iCulled;
}
/////////////////////////////////////////////////////////////////
int
GSE_ConfigRenderer::Handle_RenderPass_DebugPrint_Enter( GSE_GraphNode<GSE_ConfigItemType> *pNode )
{
  int iCulled = 0;
  GSE_ConfigOption *pConfig = (GSE_ConfigOption *)pNode;
  switch( pNode->GetType())
  {
  case CONFIGNODE_NOT_SPECIFIED:
    GSE_WARN( "GSE_ConfigRenderer : Undefined node!" );
    iCulled = 1;
    break;
  case CONFIGNODE_OPTION:
    if ( strncmp(pNode->GetName(),"ConfigRoot", 10) == 0)
    {
      m_strIndent.erase();
      GSE_LOG(m_strIndent << "ConfigRoot");
      m_strIndent += "\t";
    }
    else
    {
      if ( pNode->HasLeavingEdges()){
	GSE_LOG(m_strIndent << "Section " << pNode->GetName() );      
	m_strIndent+="\t";
      }
      else
      {
	GSE_LOG(m_strIndent 
		<< pConfig->GetName() << " = " 
		<< pConfig->GetValue());      
      }
    }
    break;
  }
  return iCulled;
}
/////////////////////////////////////////////////////////////////
void   
GSE_Configuration::SetWarningsEnabled( bool bFlag )
{
  m_bShowWarnings = bFlag;
}
/////////////////////////////////////////////////////////////////
// history : 
//		$Log: GSE_Configuration.cpp,v $
//		Revision 1.16  2007/04/01 21:06:34  entity
//		code cleanups
//
//		Revision 1.15  2007/03/12 14:53:22  entity
//		changes according to new TravelDF()
//
//		Revision 1.14  2007/03/10 14:56:17  entity
//		each config file must have at least one section to be valid.
//
//		Revision 1.13  2007/03/09 10:44:33  entity
//		GetLong() changes
//
//		Revision 1.12  2007/03/08 20:21:59  entity
//		CreateNode<> added
//
//		Revision 1.11  2007/03/05 11:30:22  entity
//		Uses now GSE_GraphNodeTraveler
//
//		Revision 1.10  2007/03/02 07:25:05  entity
//		changed the name of method
//
//		Revision 1.9  2007/03/02 07:22:23  entity
//		Support for suppressing the warnings
//
//		Revision 1.8  2007/01/08 12:24:00  entity
//		added GPL licence
//
//		Revision 1.7  2006/12/27 12:41:11  entity
//		fixed debug output
//
//		Revision 1.6  2006/10/20 16:07:07  entity
//		added changeable Section start and end strings + getBoolean()
//
//		Revision 1.5  2006/10/17 11:05:35  entity
//		removed #DEBUG
//
//		Revision 1.4  2006/10/14 22:03:04  entity
//		First fully operational version
//
//		Revision 1.3  2006/10/14 20:03:43  entity
//		some comments and warnings
//
//		Revision 1.2  2006/10/14 19:02:24  entity
//		First working version
//
//		Revision 1.1  2006/10/14 16:23:35  entity
//		Initial import to CVS
//
/////////////////////////////////////////////////////////////////
