//**************************************************************************************
// Filename:	CMUtils.cp
//				Part of Contextual Menu Workshop by Abracode Inc.
//				http://free.abracode.com/cmworkshop/
//
// Copyright © 2002-2004 Abracode, Inc.  All rights reserved.
//
// Description:	static utilities for Contextual Menu Plugins
//
//
//**************************************************************************************

#include "CMUtils.h"
#include "DebugSettings.h"

#include "MoreFilesX.h"

#if COMPILE_FSSPEC_CODE
	#include "MoreFilesExtras.h"
#endif //COMPILE_FSSPEC_CODE

//this one is based on FSDeleteContainer from MoreFilesX
//but is modified to delete any item - it may be a non-empty folder or it may be a file
OSErr
CMUtils::DeleteObject(const FSRef *inRef)
{
	if(inRef == NULL)
		return paramErr;

	FSCatalogInfo	catalogInfo;
	OSErr result = FSGetCatalogInfo(inRef, kFSCatInfoNodeFlags, &catalogInfo, NULL, NULL, NULL);
	if( result != noErr)
		return result;
	
	if( 0 != (catalogInfo.nodeFlags & kFSNodeIsDirectoryMask) )
	{
		/* delete container's contents */
		result = ::FSDeleteContainerContents(inRef);
		if( result != noErr)
			return result;
	}

	/* is container locked? */
	if ( 0 != (catalogInfo.nodeFlags & kFSNodeLockedMask) )
	{
		/* then attempt to unlock container (ignore result since FSDeleteObject will set it correctly) */
		catalogInfo.nodeFlags &= ~kFSNodeLockedMask;
		(void) ::FSSetCatalogInfo(inRef, kFSCatInfoNodeFlags, &catalogInfo);
	}
	
	/* delete the container */
	result = ::FSDeleteObject(inRef);
	return result;
}


#if COMPILE_FSSPEC_CODE

//this one is based on DeleteDirectory from MoreFiles
//but is modified to delete any item - it may be a non-empty folder or it may be a file

OSErr
CMUtils::DeleteObject(const FSSpec *inSpec)
{
	if(inSpec == NULL)
		return paramErr;

	OSErr error;
	
	Boolean isFolder = false;
	error = CMUtils::IsFolder(inSpec, isFolder);
	
	if( (error == noErr) && isFolder)
	{
		/* Make sure a directory was specified and then delete its contents */
		error = ::DeleteDirectoryContents(inSpec->vRefNum, inSpec->parID, inSpec->name);
	}

	if ( error == noErr )
	{
		error = ::HDelete(inSpec->vRefNum, inSpec->parID, inSpec->name);
		if ( error == fLckdErr )
		{
			(void) ::HRstFLock(inSpec->vRefNum, inSpec->parID, inSpec->name);	/* unlock the directory locked by AppleShare */
			error = ::HDelete(inSpec->vRefNum, inSpec->parID, inSpec->name);	/* and try again */
		}
	}

	return ( error );
}
#endif //COMPILE_FSSPEC_CODE