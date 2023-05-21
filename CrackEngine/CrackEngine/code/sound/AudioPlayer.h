#pragma once

#include <string>
#include <iostream>
#include <portaudio.h>
#include <sndfile.h>

#define FRAMES_PER_BUFFER 256

class AudioPlayer {
private:
    std::string filename;
    bool isLooping;
    float volume;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    PaStream* stream;
    sf_count_t framesRead;

    static int paCallback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags,
        void* userData) {
        AudioPlayer* player = (AudioPlayer*)userData;
        return player->callback(inputBuffer, outputBuffer, framesPerBuffer, timeInfo, statusFlags);
    }

    int callback(const void* inputBuffer, void* outputBuffer,
        unsigned long framesPerBuffer,
        const PaStreamCallbackTimeInfo* timeInfo,
        PaStreamCallbackFlags statusFlags) {
        float* out = (float*)outputBuffer;
        (void)timeInfo; /* Prevent unused variable warnings. */
        (void)statusFlags;
        (void)inputBuffer;

        // Read audio data from file
        framesRead = sf_readf_float(sndfile, out, framesPerBuffer);

        // Adjust volume
        for (int i = 0; i < framesPerBuffer * sfinfo.channels; i++) {
            out[i] *= volume;
        }

        // If the file ended and looping is enabled, seek back to the beginning of the file
        if (framesRead < framesPerBuffer && isLooping) {
            sf_seek(sndfile, 0, SEEK_SET);
        }

        return paContinue;
    }

public:
    AudioPlayer(std::string filename, bool isLooping = false, float volume = 1.0f)
        : filename(filename), isLooping(isLooping), volume(volume), sndfile(nullptr), stream(nullptr), framesRead(0) {
        Pa_Initialize();

        // Open the sound file
        sfinfo.format = 0;
        sndfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
        if (!sndfile) {
            std::cout << "Couldn't open sound file " << filename << ": " << sf_strerror(sndfile) << "\n";
            return;
        }

        // Open the PortAudio stream
        PaError err = Pa_OpenDefaultStream(&stream, 0, sfinfo.channels, paFloat32, sfinfo.samplerate, FRAMES_PER_BUFFER, &AudioPlayer::paCallback, this);
        if (err != paNoError) {
            std::cout << "Couldn't open stream: " << Pa_GetErrorText(err) << "\n";
            return;
        }
    }

    ~AudioPlayer() {
        // Close the sound file and the PortAudio stream
        if (sndfile) sf_close(sndfile);
        if (stream) {
            Pa_AbortStream(stream);
            Pa_CloseStream(stream);
        }
        Pa_Terminate();
    }

    void play() {
        if (stream) {
            sf_seek(sndfile, 0, SEEK_SET);
            Pa_StartStream(stream);
        }
    }

    void stop() {
        if (stream) {
            Pa_StopStream(stream);
        }
    }
};