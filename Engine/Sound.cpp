#include "Sound.h"

Sound::Sound()
{
    m_DirectSound = 0;
    m_primaryBuffer = 0;
    m_secondaryBuffer = 0;
}

Sound::Sound(const Sound&)
{
}

Sound::~Sound()
{
}

bool Sound::Init(HWND hwnd)
{
    bool result;

    result = InitDirectSound(hwnd);
    if (!result)
        return false;
    result = LoadWaveFile((char*)"../Engine/data/TestSound.wav", &m_secondaryBuffer);
    if (!result)
        return false;

    result = PlayWaveFile();
    if (!result)
        return false;
    return true;
}

void Sound::Shutdown()
{
    ShutDownWaveFile(&m_secondaryBuffer);
    ShutDownDirectSound();

    return;

}

bool Sound::InitDirectSound(HWND hwnd)
{
    HRESULT result;
    DSBUFFERDESC bufferDesc;
    WAVEFORMATEX waveFormat;

    result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
    if (FAILED(result))
        return false;
    result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
    if (FAILED(result))
        return false;
    bufferDesc.dwSize = sizeof(DSBUFFERDESC);
    bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    bufferDesc.dwBufferBytes = 0;
    bufferDesc.dwReserved = 0;
    bufferDesc.lpwfxFormat = NULL;
    bufferDesc.guid3DAlgorithm = GUID_NULL;

    result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
    if (FAILED(result))
        return false;
    waveFormat.wFormatTag = WAVE_FORMAT_PCM;
    waveFormat.nSamplesPerSec = 44100;
    waveFormat.wBitsPerSample = 16;
    waveFormat.nChannels = 2;
    waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
    waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
    waveFormat.cbSize = 0;

    result = m_primaryBuffer->SetFormat(&waveFormat);
    if (FAILED(result))
        return false;

    return true;
}

void Sound::ShutDownDirectSound()
{
    if (m_primaryBuffer)
    {
        m_primaryBuffer->Release();
        m_primaryBuffer = 0;
    }
    if (m_DirectSound)
    {
        m_DirectSound->Release();
        m_DirectSound = 0;
    }
    return;
}

bool Sound::LoadWaveFile(char* file, IDirectSoundBuffer8** secondaryBuffer)
{
    int error;
    FILE* filePtr;
    unsigned int count;
    WaveHeader waveFile;
    WAVEFORMATEX waveFormat;
    DSBUFFERDESC bufferDesc;
    HRESULT result;
    IDirectSoundBuffer* tempBuffer;
    unsigned char* waveData;
    unsigned char* bufferPtr;
    unsigned long bufferSize;
    
    error = fopen_s(&filePtr, file, "rb");
    if (error != 0)
        return false;
    count = fread(&waveFile, sizeof(waveFile), 1, filePtr);
    if(count != 1)
        return false;

    if((waveFile.chunkld[0] != 'R')||(waveFile.chunkld[1] != 'I')||(waveFile.chunkld[2] != 'F')||(waveFile.chunkld[3]!= 'F'))
        return false;

    if ((waveFile.format[0] != 'W') || (waveFile.format[1] != 'A') || (waveFile.format[2] != 'V') || (waveFile.format[3] != 'E'))
        return false;

    if ((waveFile.subChunk[0] != 'f') || (waveFile.subChunk[1] != 'm') || (waveFile.subChunk[2] != 't') || (waveFile.subChunk[3] != ' '))
        return false;




    return true;
}

void Sound::ShutDownWaveFile(IDirectSoundBuffer8**)
{
}

bool Sound::PlayWaveFile()
{
    return true;
}
