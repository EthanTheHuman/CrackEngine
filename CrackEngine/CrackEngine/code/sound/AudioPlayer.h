#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <filesystem>
#include <portaudio.h>
#include <sndfile.h>
#include <mpg123.h>

#define FRAMES_PER_BUFFER 256

class AudioPlayer {
private:

    enum class AudioType {
        MP3,
        OTHER
    };
		
	
    std::string filename;
    bool isLooping;
    float volume;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    PaStream* stream;
    sf_count_t framesRead;
    mpg123_handle* mh;
    unsigned char* mp3_buffer;
    AudioType audioType = AudioType::OTHER;

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

        switch (audioType)
        {
            case AudioType::MP3:
            {
                // Handle MP3 decoding and playback here with libmpg123
                // Read audio data from MP3 file
                size_t done;
                mpg123_read(mh, mp3_buffer, framesPerBuffer * 2 * sizeof(short), &done);
                short* buffer_short = (short*)mp3_buffer;
                framesRead = done / 2;

                // Convert PCM to float
                for (int i = 0; i < framesRead; i++) {
                    out[i] = buffer_short[i] / 32768.0f * volume;
                }

                // If looping and file ended, seek to the beginning
                if (framesRead < framesPerBuffer && isLooping) {
                    mpg123_seek(mh, 0, SEEK_SET);
                }
                break;
            }
            case AudioType::OTHER:
            {
                // Handle WAV decoding and playback as before with libsndfile

                // Read audio data from file
                framesRead = sf_readf_float(sndfile, out, framesPerBuffer);

                // Adjust volume
                for (int i = 0; i < framesPerBuffer * sfinfo.channels; i++) {
                    out[i] *= volume;
                }

                // If the file ended and looping is enabled, seek back to the beginning of the file
                if (framesRead < framesPerBuffer && isLooping) {
                    switch (audioType)
                    {
                        case AudioType::MP3:
                        {
                            mpg123_seek(mh, 0, SEEK_SET);
                            break;
                        }
                        case AudioType::OTHER:
                        {
                            sf_seek(sndfile, 0, SEEK_SET);
                        }
                    }
                }
                break;
            }
        }

        return paContinue;
    }
	
public:
    AudioPlayer(std::string directory, bool isLooping = false, float volume = 1.0f)
        : isLooping(isLooping), volume(volume), sndfile(nullptr), stream(nullptr), framesRead(0), mh(nullptr), mp3_buffer(nullptr) {
        filename = directory;

        Pa_Initialize();
        mpg123_init();
        mh = mpg123_new(nullptr, nullptr);
        mpg123_open(mh, filename.c_str());
        long rate;
        int channels, encoding;
        mpg123_getformat(mh, &rate, &channels, &encoding);
		
        // Determine if the file is mp3
        if (filename.substr(filename.size() - 4) == ".mp3") {
            audioType = AudioType::MP3;
            mp3_buffer = new unsigned char[FRAMES_PER_BUFFER * 2 * sizeof(short)];
            sfinfo.samplerate = rate;
            sfinfo.channels = channels;
        }
        else {
            // Open the sound file with libsndfile
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
        if (mh) {
            mpg123_close(mh);
            mpg123_delete(mh);
            mpg123_exit();
            delete[] mp3_buffer;
        }
        if (stream) {
            Pa_AbortStream(stream);
            Pa_CloseStream(stream);
        }
        Pa_Terminate();
    }

    void play() {
        if (stream) {
            switch (audioType)
            {
                case AudioType::MP3:
                {
                    mpg123_seek(mh, 0, SEEK_SET);
                    break;
                }
                case AudioType::OTHER:
                {
                    sf_seek(sndfile, 0, SEEK_SET);
                }
            }
            Pa_StartStream(stream);
        }
    }

    void stop() {
        if (stream) {
            Pa_StopStream(stream);
        }
    }

    static std::string randomFileFromDirectory(const std::string& directory) {
        std::vector<std::string> files;
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            files.push_back(entry.path().string());
        }
        if (files.empty()) return "";
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(0, files.size() - 1);
        return files[distr(gen)];
    }
};
