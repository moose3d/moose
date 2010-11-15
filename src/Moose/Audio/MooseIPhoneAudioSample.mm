/////////////////////////////////////////////////////////////////////////////
#include "MooseIPhoneAudioSample.h"
#include "MooseLogger.h"
#include "MooseDefaultEntities.h"
#import <Foundation/NSString.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AudioToolbox/ExtendedAudioFile.h>
#include <string>
using namespace std;
void* GetOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*	outSampleRate);
/////////////////////////////////////////////////////////////////////////////
bool 
Moose::Sound::CIPhoneAudioSample::Load( const char *szFilename )
{
    NSString *fileString = [NSString stringWithUTF8String:szFilename];      
    NSLog(@"our file url is '%@'\n",fileString);
    // get some audio data from a wave file
    //CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:nsFile] retain];
    CFURLRef fileURL = CFURLCreateWithString(kCFAllocatorDefault, (CFStringRef)fileString, NULL);
    
    
     m_pBufferData = reinterpret_cast<char *>( GetOpenALAudioData(fileURL, &m_Size, &m_Format, &m_Freq) ); 
     CFRelease(fileURL);
     ALenum error = alGetError();
    if ( error != AL_NO_ERROR ) {
        cerr << GetOpenALErrorString(error) << endl;
        return false;
    } 
    alBufferData( GetALObject(), GetFormat(), m_pBufferData, m_Size, GetFreq());
    error = alGetError();
    if ( error != AL_NO_ERROR )
    {
        cerr << GetOpenALErrorString(error) << endl;
        return false;
    }
    return true;
}
/////////////////////////////////////////////////////////////////////////////
void* GetOpenALAudioData(CFURLRef inFileURL, ALsizei *outDataSize, ALenum *outDataFormat, ALsizei*	outSampleRate)
{
  OSStatus                      err = noErr;
  SInt64                        theFileLengthInFrames = 0;
  AudioStreamBasicDescription   theFileFormat;
  UInt32                        thePropertySize = sizeof(theFileFormat);
  ExtAudioFileRef               extRef = NULL;
  void*                         theData = NULL;
  AudioStreamBasicDescription   theOutputFormat;
  UInt32                        dataSize;
  // Open a file with ExtAudioFileOpen()
  err = ExtAudioFileOpenURL(inFileURL, &extRef);
  if(err) { printf("OpenAL: ExtAudioFileOpenURL FAILED, Error = %ld\n", err); goto Exit; }
    
  // Get the audio data format
  err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
  if(err) { printf("OpenAL: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", err); goto Exit; }
  if (theFileFormat.mChannelsPerFrame > 2)  { printf("OpenAL - Unsupported Format, channel count is greater than stereo\n"); goto Exit;}
    
  // Set the client format to 16 bit signed integer (native-endian) data
  // Maintain the channel count and sample rate of the original source format
  theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
  theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
    
  theOutputFormat.mFormatID = kAudioFormatLinearPCM;
  theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
  theOutputFormat.mFramesPerPacket = 1;
  theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
  theOutputFormat.mBitsPerChannel = 16;
  theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
    
  // Set the desired client (output) data format
  err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
  if(err) { printf("MyGetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ld\n", err); goto Exit; }
    
  // Get the total frame count
  thePropertySize = sizeof(theFileLengthInFrames);
  err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
  if(err) { printf("MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ld\n", err); goto Exit; }
    
  // Read all the data into memory
  dataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;;
  theData = malloc(dataSize);
  if (theData)
  {
    AudioBufferList		theDataBuffer;
    theDataBuffer.mNumberBuffers = 1;
    theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
    theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
    theDataBuffer.mBuffers[0].mData = theData;
        
    // Read the data into an AudioBufferList
    err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
    if(err == noErr)
    {
      // success
      *outDataSize = (ALsizei)dataSize;
      *outDataFormat = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
      *outSampleRate = (ALsizei)theOutputFormat.mSampleRate;
    }
    else
    {
      // failure
      free (theData);
      theData = NULL; // make sure to return NULL
      printf("OpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err); goto Exit;
    }
  }
    
 Exit:
  // Dispose the ExtAudioFileRef, it is no longer needed
  if (extRef) ExtAudioFileDispose(extRef);
  return theData;
}
