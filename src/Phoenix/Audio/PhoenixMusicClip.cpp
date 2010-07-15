/*
 *  PhoenixMusicClip.cpp
 *  iPhoneMoose
 *
 *  Created by eNtity on 5/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "PhoenixMusicClip.h"
#include "PhoenixLogger.h"
#include "PhoenixDefaultEntities.h"
#define PRINTERROR(LABEL)	printf("%s err %4.4s %d\n", LABEL, &err, err)
using namespace std;
using namespace Phoenix::Sound;
const size_t BUFFER_SIZE = 40000;
const size_t SLEEP_TIME_ON_PAUSE = 5000; // microseconds
/////////////////////////////////////////////////////////////////
void MyAudioQueueOutputCallback(void* inClientData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);
void MyAudioQueueIsRunningCallback(void *inUserData, AudioQueueRef inAQ, AudioQueuePropertyID inID);

void MyPropertyListenerProc(	void *							inClientData,
                            AudioFileStreamID				inAudioFileStream,
                            AudioFileStreamPropertyID		inPropertyID,
                            UInt32 *						ioFlags);

void MyPacketsProc(				void *							inClientData,
                   UInt32							inNumberBytes,
                   UInt32							inNumberPackets,
                   const void *					inInputData,
                   AudioStreamPacketDescription	*inPacketDescriptions);

OSStatus MyEnqueueBuffer(MyData* myData);
void WaitForFreeBuffer(MyData* myData);
//////////////////////////////////////////////////////////////
bool
Phoenix::Sound::CMusicClip::IsPaused() const
{
    return m_bPaused;
}
//////////////////////////////////////////////////////////////
bool
Phoenix::Sound::CMusicClip::IsRunning() const
{
    return m_bRunning;
}
//////////////////////////////////////////////////////////////
Phoenix::Sound::CMusicClip::CMusicClip() : m_pData(NULL), m_bRunning(false), m_bPaused(false), m_bHasThread(false)
{
    
}//////////////////////////////////////////////////////////////
Phoenix::Sound::CMusicClip::~CMusicClip()
{
    if ( m_bHasThread )
    {
        void * status;
        g_Log << "MusicClip: joining threads..."  << endl;
        pthread_join( m_thread, &status );
        g_Log << "MusicClip: thread join ok"<< endl;
        m_bHasThread = false;
    }
    // cleanup
	free(buf);
	OSStatus err = AudioFileStreamClose(m_pData->audioFileStream);
	err = AudioQueueDispose(m_pData->audioQueue, false);
    
    if ( m_pData ) 
        free(m_pData);
    m_pData = NULL;
}
/////////////////////////////////////////////////////////////
bool
Phoenix::Sound::CMusicClip::Load( const char *szFilename )
{
    if ( m_pData ) {
        g_Error << "Already loaded" << endl;
        return false;
    }
    // allocate a struct for storing our state
	m_pData = (MyData*)calloc(1, sizeof(MyData));
    // initialize a mutex and condition so that we can block on buffers in use.
	pthread_mutex_init(&m_pData->mutex, NULL);
	pthread_cond_init(&m_pData->cond, NULL);
	pthread_cond_init(&m_pData->done, NULL);
	
	// allocate a buffer for reading data from a socket
	buf = (char*)malloc(BUFFER_SIZE * sizeof(char));
    
    m_File.open( szFilename, ios::binary);
    if ( m_File.is_open() == false ) 
    {
        g_Error << "Could not open file : '" << szFilename << "'\n";
        return false;
    }
	// create an audio file stream parser
	OSStatus err = AudioFileStreamOpen(m_pData, MyPropertyListenerProc, MyPacketsProc, 
                                       kAudioFileMP3Type, &m_pData->audioFileStream);
	if (err) { g_Error << "AudioFileStreamOpen" << endl; return false; }
    return true;
}
/////////////////////////////////////////////////////////////
void *
audio_decode_play_proc( void * pClientData )
{
    CMusicClip * pObj = reinterpret_cast<CMusicClip *> (pClientData);
    pObj->m_bRunning = true;
    pObj->m_bHasThread = true;
    while ( !pObj->m_pData->failed && pObj->IsRunning() ) 
    {
               if ( pObj->IsPaused() )
               {
                   usleep(SLEEP_TIME_ON_PAUSE);
               }
               else 
               {
               // read data from the socket
               printf("->recv\n");
               
               ssize_t bytesRecvd = pObj->m_File.readsome(pObj->buf, BUFFER_SIZE);
               printf("bytesRecvd %d\n", bytesRecvd);
               if (bytesRecvd <= 0) break; // eof or failure
               
               // parse the data. this will call MyPropertyListenerProc and MyPacketsProc
               OSStatus err = AudioFileStreamParseBytes(pObj->m_pData->audioFileStream, bytesRecvd, pObj->buf, 0);
               if (err) { PRINTERROR("AudioFileStreamParseBytes"); break; }
               }
    }
    if ( pObj->IsRunning() == false)
    {
        AudioQueueStop( pObj->m_pData->audioQueue, TRUE);
        pthread_exit(NULL);
    } 
    else {
        // enqueue last buffer
        MyEnqueueBuffer(pObj->m_pData);
           
        printf("flushing\n");
        OSStatus err = AudioQueueFlush(pObj->m_pData->audioQueue);
        if (err) { PRINTERROR("AudioQueueFlush"); }	
           
        pObj->Stop();
    
        printf("waiting until finished playing..\n");
        pthread_mutex_lock(&pObj->m_pData->mutex); 
        pthread_cond_wait(&pObj->m_pData->done, &pObj->m_pData->mutex);
        pthread_mutex_unlock(&pObj->m_pData->mutex);
           
           
        printf("done\n");
        pObj->m_bRunning = false;
        pthread_exit(NULL);
    }
    return NULL;
}
/////////////////////////////////////////////////////////////
void
Phoenix::Sound::CMusicClip::Play()
{
    
 if ( IsPaused())
 {
     m_bPaused = false;
     pthread_mutex_lock(&m_pData->mutex); 
     AudioQueueStart(m_pData->audioQueue, NULL);
     pthread_mutex_unlock(&m_pData->mutex);
 }
 else 
 {
     if ( IsRunning()) Stop();
     if ( m_bHasThread ) 
     {
         void *status;
         pthread_join(m_thread, &status);
         m_bHasThread = false;
     }
     OSStatus err = pthread_create( &m_thread, NULL, audio_decode_play_proc, this);
     if ( err ) {
         g_Error << "Could not create thread for music clip, error: " << err << endl;
         
     }
    }
      
	
	    
    
}
/////////////////////////////////////////////////////////////
void
Phoenix::Sound::CMusicClip::Stop()
{
    m_bRunning = false;
    //pthread_mutex_lock(&m_pData->mutex); 
	AudioQueueStop(m_pData->audioQueue, TRUE);
    //pthread_mutex_unlock(&m_pData->mutex);
}
/////////////////////////////////////////////////////////////
void
Phoenix::Sound::CMusicClip::Pause()
{
    m_bPaused = true;
    //pthread_mutex_lock(&m_pData->mutex); 
    AudioQueuePause(m_pData->audioQueue);
    //pthread_mutex_unlock(&m_pData->mutex);
}
/////////////////////////////////////////////////////////////




void MyPropertyListenerProc(	void *							inClientData,
                            AudioFileStreamID				inAudioFileStream,
                            AudioFileStreamPropertyID		inPropertyID,
                            UInt32 *						ioFlags)
{	
	// this is called by audio file stream when it finds property values
	MyData* myData = (MyData*)inClientData;
	OSStatus err = noErr;
    
	printf("found property '%c%c%c%c'\n", (inPropertyID>>24)&255, (inPropertyID>>16)&255, (inPropertyID>>8)&255, inPropertyID&255);
    
	switch (inPropertyID) {
		case kAudioFileStreamProperty_ReadyToProducePackets :
		{
			// the file stream parser is now ready to produce audio packets.
			// get the stream format.
			AudioStreamBasicDescription asbd;
			UInt32 asbdSize = sizeof(asbd);
			err = AudioFileStreamGetProperty(inAudioFileStream, kAudioFileStreamProperty_DataFormat, &asbdSize, &asbd);
			if (err) { PRINTERROR("get kAudioFileStreamProperty_DataFormat"); myData->failed = true; break; }
			
			// create the audio queue
			err = AudioQueueNewOutput(&asbd, MyAudioQueueOutputCallback, myData, NULL, NULL, 0, &myData->audioQueue);
			if (err) { PRINTERROR("AudioQueueNewOutput"); myData->failed = true; break; }
			
			// allocate audio queue buffers
			for (unsigned int i = 0; i < kNumAQBufs; ++i) {
				err = AudioQueueAllocateBuffer(myData->audioQueue, kAQBufSize, &myData->audioQueueBuffer[i]);
				if (err) { PRINTERROR("AudioQueueAllocateBuffer"); myData->failed = true; break; }
			}
            
			// get the cookie size
			UInt32 cookieSize;
			Boolean writable;
			err = AudioFileStreamGetPropertyInfo(inAudioFileStream, kAudioFileStreamProperty_MagicCookieData, &cookieSize, &writable);
			if (err) { PRINTERROR("info kAudioFileStreamProperty_MagicCookieData"); break; }
			printf("cookieSize %d\n", cookieSize);
            
			// get the cookie data
			void* cookieData = calloc(1, cookieSize);
			err = AudioFileStreamGetProperty(inAudioFileStream, kAudioFileStreamProperty_MagicCookieData, &cookieSize, cookieData);
			if (err) { PRINTERROR("get kAudioFileStreamProperty_MagicCookieData"); free(cookieData); break; }
            
			// set the cookie on the queue.
			err = AudioQueueSetProperty(myData->audioQueue, kAudioQueueProperty_MagicCookie, cookieData, cookieSize);
			free(cookieData);
			if (err) { PRINTERROR("set kAudioQueueProperty_MagicCookie"); break; }
            
			// listen for kAudioQueueProperty_IsRunning
			err = AudioQueueAddPropertyListener(myData->audioQueue, kAudioQueueProperty_IsRunning, MyAudioQueueIsRunningCallback, myData);
			if (err) { PRINTERROR("AudioQueueAddPropertyListener"); myData->failed = true; break; }
			
			break;
		}
	}
}

void MyPacketsProc(				void *							inClientData,
                   UInt32							inNumberBytes,
                   UInt32							inNumberPackets,
                   const void *					inInputData,
                   AudioStreamPacketDescription	*inPacketDescriptions)
{
	// this is called by audio file stream when it finds packets of audio
	MyData* myData = (MyData*)inClientData;
	printf("got data.  bytes: %d  packets: %d\n", inNumberBytes, inNumberPackets);
    
	// the following code assumes we're streaming VBR data. for CBR data, you'd need another code branch here.
    
	for (int i = 0; i < inNumberPackets; ++i) {
		SInt64 packetOffset = inPacketDescriptions[i].mStartOffset;
		SInt64 packetSize   = inPacketDescriptions[i].mDataByteSize;
		
		// if the space remaining in the buffer is not enough for this packet, then enqueue the buffer.
		size_t bufSpaceRemaining = kAQBufSize - myData->bytesFilled;
		if (bufSpaceRemaining < packetSize) {
			MyEnqueueBuffer(myData);
			WaitForFreeBuffer(myData);
		}
		
		// copy data to the audio queue buffer
		AudioQueueBufferRef fillBuf = myData->audioQueueBuffer[myData->fillBufferIndex];
		memcpy((char*)fillBuf->mAudioData + myData->bytesFilled, (const char*)inInputData + packetOffset, packetSize);
		// fill out packet description
		myData->packetDescs[myData->packetsFilled] = inPacketDescriptions[i];
		myData->packetDescs[myData->packetsFilled].mStartOffset = myData->bytesFilled;
		// keep track of bytes filled and packets filled
		myData->bytesFilled += packetSize;
		myData->packetsFilled += 1;
		
		// if that was the last free packet description, then enqueue the buffer.
		size_t packetsDescsRemaining = kAQMaxPacketDescs - myData->packetsFilled;
		if (packetsDescsRemaining == 0) {
			MyEnqueueBuffer(myData);
			WaitForFreeBuffer(myData);
		}
	}	
}

OSStatus StartQueueIfNeeded(MyData* myData)
{
	OSStatus err = noErr;
	if (!myData->started) {		// start the queue if it has not been started already
		err = AudioQueueStart(myData->audioQueue, NULL);
		if (err) { PRINTERROR("AudioQueueStart"); myData->failed = true; return err; }		
		myData->started = true;
		printf("started\n");
	}
	return err;
}

OSStatus MyEnqueueBuffer(MyData* myData)
{
	OSStatus err = noErr;
	myData->inuse[myData->fillBufferIndex] = true;		// set in use flag
	
	// enqueue buffer
	AudioQueueBufferRef fillBuf = myData->audioQueueBuffer[myData->fillBufferIndex];
	fillBuf->mAudioDataByteSize = myData->bytesFilled;		
	err = AudioQueueEnqueueBuffer(myData->audioQueue, fillBuf, myData->packetsFilled, myData->packetDescs);
	if (err) { PRINTERROR("AudioQueueEnqueueBuffer"); myData->failed = true; return err; }		
	
	StartQueueIfNeeded(myData);
	
	return err;
}


void WaitForFreeBuffer(MyData* myData)
{
	// go to next buffer
	if (++myData->fillBufferIndex >= kNumAQBufs) myData->fillBufferIndex = 0;
	myData->bytesFilled = 0;		// reset bytes filled
	myData->packetsFilled = 0;		// reset packets filled
    
	// wait until next buffer is not in use
	printf("->lock\n");
	pthread_mutex_lock(&myData->mutex); 
	while (myData->inuse[myData->fillBufferIndex]) {
		printf("... WAITING ...\n");
		pthread_cond_wait(&myData->cond, &myData->mutex);
	}
	pthread_mutex_unlock(&myData->mutex);
	printf("<-unlock\n");
}

int MyFindQueueBuffer(MyData* myData, AudioQueueBufferRef inBuffer)
{
	for (unsigned int i = 0; i < kNumAQBufs; ++i) {
		if (inBuffer == myData->audioQueueBuffer[i]) 
			return i;
	}
	return -1;
}


void MyAudioQueueOutputCallback(	void*					inClientData, 
                                AudioQueueRef			inAQ, 
                                AudioQueueBufferRef		inBuffer)
{
	// this is called by the audio queue when it has finished decoding our data. 
	// The buffer is now free to be reused.
	MyData* myData = (MyData*)inClientData;
    
	unsigned int bufIndex = MyFindQueueBuffer(myData, inBuffer);
	
	// signal waiting thread that the buffer is free.
	pthread_mutex_lock(&myData->mutex);
	myData->inuse[bufIndex] = false;
	pthread_cond_signal(&myData->cond);
	pthread_mutex_unlock(&myData->mutex);
}

void MyAudioQueueIsRunningCallback(		void*					inClientData, 
                                   AudioQueueRef			inAQ, 
                                   AudioQueuePropertyID	inID)
{
	MyData* myData = (MyData*)inClientData;
	
	UInt32 running;
	UInt32 size;
	OSStatus err = AudioQueueGetProperty(inAQ, kAudioQueueProperty_IsRunning, &running, &size);
	if (err) { PRINTERROR("get kAudioQueueProperty_IsRunning"); return; }
	if (!running) {
		pthread_mutex_lock(&myData->mutex);
		pthread_cond_signal(&myData->done);
		pthread_mutex_unlock(&myData->mutex);
	}
}


