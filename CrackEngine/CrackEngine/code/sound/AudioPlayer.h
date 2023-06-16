#pragma once

#include <string>
#include <iostream>
#include <random>
#include <vector>
#include <filesystem>
#include <portaudio.h>
#include <sndfile.h>
#include <mpg123.h>

#define FRAMES_PER_BUFFER 256

namespace fs = std::filesystem;

class AudioPlayer {
private:
    std::string filename;
    bool isLooping;
    float volume;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    PaStream* stream;
    sf_count_t framesRead;
    mpg123_handle* mpg123;
    unsigned char* buffer;

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

        if (sndfile) {
            // Read audio data from WAV file
            framesRead = sf_readf_float(sndfile, out, framesPerBuffer);
        }
        else if (mpg123) {
            // Read audio data from MP3 file
            size_t done;
            mpg123_read(mpg123, buffer, framesPerBuffer * 2 * sizeof(float), &done);
            framesRead = done / (2 * sizeof(float));
            float* buffer_float = (float*)buffer;
            for (int i = 0; i < framesRead; i++) {
                out[i] = buffer_float[i];
            }
        }

        // Adjust volume
        for (int i = 0; i < framesPerBuffer * sfinfo.channels; i++) {
            out[i] *= volume;
        }

        // If the file ended and looping is enabled, seek back to the beginning of the file
        if (framesRead < framesPerBuffer && isLooping) {
            if (sndfile) {
                sf_seek(sndfile, 0, SEEK_SET);
            }
            else if (mpg123) {
                mpg123_seek(mpg123, 0, SEEK_SET);
            }
        }

        return paContinue;
    }

public:
    AudioPlayer(std::string filename, bool isLooping = false, float volume = 1.0f)
        : filename(filename), isLooping(isLooping), volume(volume), sndfile(nullptr), stream(nullptr), framesRead(0), mpg123(nullptr), buffer(nullptr) {
        Pa_Initialize();
        mpg123_init();
        mpg123 = mpg123_new(nullptr, nullptr);
        mpg123_open(mpg123, filename.c_str());
        long rate;
        int channels, encoding;
        mpg123_getformat(mpg123, &rate, &channels, &encoding);

        // Check if it is an MP3 file
        if (mpg123) {
            buffer = new unsigned char[FRAMES_PER_BUFFER * 2 * sizeof(float)];
            sfinfo.samplerate = rate;
            sfinfo.channels = channels;
        }
        else {
            // Open the sound file
            sfinfo.format = 0;
            sndfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
            if (!sndfile) {
                std::cout << "Couldn't open sound file " << filename << ": " << sf_strerror(sndfile) << "\n";
                return;
            }
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
        if (mpg123) {
            mpg123_close(mpg123);
            mpg123_delete(mpg123);
            mpg123_exit();
            delete[] buffer;
        }
        if (stream) {
            Pa_AbortStream(stream);
            Pa_CloseStream(stream);
        }
        Pa_Terminate();
    }

    void play() {
        if (stream) {
            if (sndfile) sf_seek(sndfile, 0, SEEK_SET);
            if (mpg123) mpg123_seek(mpg123, 0, SEEK_SET);
            Pa_StartStream(stream);
        }
    }

    void stop() {
        if (stream) {
            Pa_StopStream(stream);
        }
    }

    static std::string randomFileFromDirectory(const std::string& directoryPath) {
        std::vector<std::string> files;

        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.is_regular_file() && (entry.path().extension() == ".wav" || entry.path().extension() == ".mp3")) {
                files.push_back(entry.path().string());
            }
        }

        if (files.empty()) {
            return "";
        }

        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<size_t> dist(0, files.size() - 1);

        return files[dist(mt)];
    }
};