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
#ifndef __GSE_3dsTools_h__
#define __GSE_3dsTools_h__
/////////////////////////////////////////////////////////////////
#include "GSE_structures.h"
#include "GSE_GraphNodes.h"
#include <string>
/////////////////////////////////////////////////////////////////
using std::string;
/////////////////////////////////////////////////////////////////
/// A class which convertes .3ds models into rendergraph.
class GSE_3dsConverter : public GSE_Singleton<GSE_3dsConverter>
{
  friend class GSE_Singleton<GSE_3dsConverter>;
 protected:
  GSE_3dsConverter()  {}
  ~GSE_3dsConverter() {}
 public:
  /////////////////////////////////////////////////////////////////
  /// The principal conversion routine.
  int Convert( const string &sFilename,  
	       const string &sNodename,  
	       GSE_RenderGraph *pRenderGraph,     
	       GSE_GeometryNode **ppGeometryNode );

};
/////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////
// $Log: GSE_3dsTools.h,v $
// Revision 1.1  2007/03/27 10:12:59  entity
// 3dstools added
//
/////////////////////////////////////////////////////////////////
