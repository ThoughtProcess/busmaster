/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file      ParameterVal.cpp
 * \brief     Implementation file for the ParameterValues class.
 * \author    Padmaja A
 * \copyright Copyright (c) 2011, Robert Bosch Engineering and Business Solutions. All rights reserved.
 *
 * Implementation file for the ParameterValues class.
 */

#include "StdAfx.h"
#include "ParameterVal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
 * Constructor of CParameterValues
 */
CParameterValues::CParameterValues()
{
    m_NodeName[0]='\0';
    m_MsgId=0;
    m_ParamVal.dValue =0;
    m_SignalName[0]='\0';
    m_cFrameFormat=MSG_FF_STANDARD;
    m_ParamVal.iValue =-1;
    m_ParamVal.fValue=-1;
    m_ParamVal.uiValue =-1;
}

/**
 * destructor of CParameterValues
 */
CParameterValues::~CParameterValues()
{

}

/**
 * overloaded operator =
 */
CParameterValues& CParameterValues::operator=(CParameterValues& param)
{
    // now copy the other elements of the new message to this
    strcpy(m_NodeName,param.m_NodeName);
    strcpy(m_SignalName,param.m_SignalName );
    m_MsgId=param.m_MsgId;
    m_ParamVal=param.m_ParamVal;
    m_cFrameFormat=param.m_cFrameFormat;
    return (*this);
}

/**
 * Parses the Parameter Values other than Default value
 * from the input file.
 */
void CParameterValues::Format_Param_Value(char *paramType,char *pcLine,const int& index,UINT msgId,char *Node_Name)
{
    //copies the node name to the member
    if(index==1)
        strcpy(m_NodeName,Node_Name);
    //copies the message id.
    else if(index==2)
        m_MsgId=msgId;
    //store the signal name.
    else if(index==3)
    {
        m_MsgId =msgId;
        strcpy(m_SignalName,Node_Name);
    }
    //get param value.
    ReadParamValue(paramType,pcLine);
}

/**
 * Reads the other vlaue of attributes from i/p file.
 */
void CParameterValues::ReadParamValue(char *paramType,char *pcToken)
{
    char acTemp[defCON_CHAR_LEN],*pcTemp;
    pcTemp = acTemp;
    //get param value of type String
    if(strcmp(paramType,"STRING")==0 )
    {   pcToken++;
        while(*pcToken != '"')
        {
            *pcTemp++=*pcToken++;
        }
        *pcTemp='\0';
        strcpy(m_ParamVal.cValue,acTemp);
        pcTemp=acTemp;
    }
    //get param value of type int
    else if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(strcmp(pcToken," ")!=0)
            m_ParamVal.iValue=atoi(pcToken);
    }
    //get param value of type enum
    else if (strcmp(paramType,"ENUM")==0)
    {
        *pcToken++;
        while(*pcToken!='"')
        {
            *pcTemp++=*pcToken++;
        }
        *pcTemp='\0';
        strcpy(m_ParamVal.cValue,acTemp);
        pcTemp=acTemp;
    }
    //get param value of type float
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(strcmp(pcToken," ")!=0)
            m_ParamVal.fValue=float(atof(pcToken));
    }

}

/**
 * Write the net values into the file
 */
void CParameterValues::WriteNetValuesToFile(fstream& fileOutput,char *paramType,char *paramName)
{
	fileOutput << "BA_ \"" << paramName << "\"";
	//writes net values of type int to o/p file.
    if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue != -1)
			fileOutput << " " << dec << m_ParamVal.iValue;
    }
    //writes net values of type float to o/p file.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue != -1)
            fileOutput << " " << fixed << m_ParamVal.fValue;
    }
    //writes net values of type enum to o/p file.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"") == 0)
			fileOutput << " \"\"";
        else
			fileOutput << " " << m_ParamVal.cValue;
    }
    //writes net values of type string to o/p file.
    else if(strcmp(paramType,"STRING")==0)
		fileOutput << " \"" << m_ParamVal.cValue << "\"";
	fileOutput << ";" << endl;
}

/**
 * Write the node values into the file
 */
void CParameterValues::WriteNodeValuesToFile(fstream& fileOutput,char *paramType,char *paramName)
{
	fileOutput << "BA_ \"" << paramName << "\"";
	fileOutput << " BU_ " << m_NodeName;
    //writes node values of type int/hex to o/p file.
    if(strcmp(paramType,"INT")==0  || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue != -1)
			fileOutput << " " << dec << m_ParamVal.iValue;
    }
    //writes node values of type enum to o/p file.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue,"") == 0)
			fileOutput << " \"\"";
        else
			fileOutput << " \"" << m_ParamVal.cValue << "\"";
    }
    //writes node values of type float to o/p file.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue != -1)
			fileOutput << fixed << m_ParamVal.fValue;
    }
    //writes node values of type string to o/p file.
    else if(strcmp(paramType,"STRING")==0)
		fileOutput << " \"" << m_ParamVal.cValue << "\"";
	fileOutput << ";" << endl;
}

/**
 * Write the Message values into the file
 */
void CParameterValues::WriteMesgValuesToFile(fstream& fileOutput,char *paramType,char *paramName)
{
	fileOutput << "BA_ \"" << paramName << "\"";
	fileOutput << " BO_ " << dec << m_MsgId;
    //writes mesg values of type int/hex to o/p file.
    if(strcmp(paramType,"INT")==0   || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue != -1)
			fileOutput << " " << dec << m_ParamVal.iValue;
    }
    //writes mesg values of type enum to o/p file.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue, "") == 0)
			fileOutput << " \"\"";
        else
			fileOutput << " " << m_ParamVal.cValue;
    }
    //writes mesg values of type float to o/p file.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue != -1)
			fileOutput << " " << fixed << m_ParamVal.fValue;
    }
    //writes mesg values of type string to o/p file.
    else if(strcmp(paramType,"STRING")==0)
		fileOutput << " \"" << m_ParamVal.cValue << "\"";
	fileOutput << ";" << endl;
}

/**
 * Write the Signal values into the file
 */
void CParameterValues::WriteSigValuesToFile(fstream& fileOutput,char *paramType,char *paramName)
{
	fileOutput << "BA_ \"" << paramName << "\"";
	fileOutput << " SG_ " << dec << m_MsgId;
	fileOutput << " " << m_SignalName;
    //writes sig values of type int/hex to o/p file.
    if(strcmp(paramType,"INT")==0 || strcmp(paramType,"HEX")==0)
    {
        if(m_ParamVal.iValue != -1)
			fileOutput << " " << dec << m_ParamVal.iValue;
    }
    //writes sig values of type enum to o/p file.
    else if(strcmp(paramType,"ENUM")==0)
    {
        if(strcmp(m_ParamVal.cValue, "") == 0)
			fileOutput << " \"\"";
        else
			fileOutput << " " << m_ParamVal.cValue;
    }
    //writes sig values of type flaot to o/p file.
    else if(strcmp(paramType,"FLOAT")==0)
    {
        if(m_ParamVal.fValue != -1)
			fileOutput << " " << fixed << m_ParamVal.fValue;
    }
    //writes sig values of type string to o/p file.
    else if(strcmp(paramType,"STRING")==0)
		fileOutput << " \"" << m_ParamVal.cValue << "\"";
	fileOutput << ";" << endl;
}
