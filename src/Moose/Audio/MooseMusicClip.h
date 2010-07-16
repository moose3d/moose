#ifndef __MooseMusicClip_h__
#define __MooseMusicClip_h__
/*
 *  MooseMusicClip.h
 *  iPhoneMoose
 *
 *  Created by eNtity on 5/12/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <AudioToolbox/AudioToolbox.h>
#include <fstream>
namespace Moose
{
    namespace Sound
    {
        ////////////////////////////////////////////////////////////////////////////////////
        const unsigned int kNumAQBufs = 3;			// number of audio queue buffers we allocate
        const size_t kAQBufSize = 128 * 1024;		// number of bytes in each audio queue buffer
        const size_t kAQMaxPacketDescs = 512;		// number of packet descriptions in our array
        //////////////////////////////////////////////////////////////////////////////////////
        struct MyData
        {
            AudioFileStreamID audioFileStream;	// the audio file stream parser
            
            AudioQueueRef audioQueue;								// the audio queue
            AudioQueueBufferRef audioQueueBuffer[kNumAQBufs];		// audio queue buffers
            
            AudioStreamPacketDescription packetDescs[kAQMaxPacketDescs];	// packet descriptions for enqueuing audio
            
            unsigned int fillBufferIndex;	// the index of the audioQueueBuffer that is being filled
            size_t bytesFilled;				// how many bytes have been filled
            size_t packetsFilled;			// how many packets have been filled
            
            bool inuse[kNumAQBufs];			// flags to indicate that a buffer is still in use
            bool started;					// flag to indicate that the queue has been started
            bool failed;					// flag to indicate an error occurred
            
            pthread_mutex_t mutex;			// a mutex to protect the inuse flags
            pthread_cond_t cond;			// a condition varable for handling the inuse flags
            pthread_cond_t done;			// a condition varable for handling the inuse flags
        };
        typedef struct MyData MyData;
        ////////////////////////////////////////////////////////////////////////////////////
        class CMusicClip
        {
        public: /* bad design, fix it. */
            MyData *        m_pData;
            std::ifstream   m_File;
            bool            m_bRunning;
            bool            m_bPaused;

            pthread_t       m_thread;
            bool            m_bHasThread;
            char *          buf;
        public:
            CMusicClip();
            virtual ~CMusicClip();
            bool Load( const char *szFilename );
            void Play();
            void Stop();
            void Pause();
            bool IsRunning() const;
            bool IsPaused() const;
        };
        ////////////////////////////////////////////////////////////////////////////////////
    }
}
#endif