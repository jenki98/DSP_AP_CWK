try:
    import google.colab
    import subprocess
    import os.path
    p = subprocess.run(['git', 'rev-parse', '--is-inside-work-tree'], stdout=subprocess.PIPE, universal_newlines=True)
    if p.stdout == 'true\n':
        !git pull
    else:
        if not os.path.isdir('city_dsp_ap'):
            !git clone --depth 1 -q https://github.com/tweyde/city_dsp_ap.git
        %cd city_dsp_ap
except:
    %cd city_dsp_ap


import numpy as np
from scipy import signal, fft
from scipy.io import wavfile
import matplotlib.pyplot as plt
from dsp_ap.operations import circ_convolve
from IPython.display import display, Audio
from dsp_ap.signals import *


plt.rcParams['figure.figsize'] = (13,6) # increase the default size of the plots


#Getting samplerate and audio samples of the carrier.wav audio file
samplerate, audio_samples = wavfile.read('audio/carrier.wav')
display(Audio(audio_samples, rate=samplerate))

#This is the starting coefficients for our filter in task 1 and will be using this to show the filter using FFT
b = [0.01, 0.01, 0.01, 0.01]

#This is using the coefficients to calculate the frequency response setting the coefficients 
# denominator as 0 and worN as the length of the spectrum
w, h = signal.freqz(b, a=1, worN=17309)
freqs = w / (100*np.pi)
freqs *= samplerate
   
#This is implementing the FFT and then applying the filtering
spectrum = fft.rfft(audio_samples)

#Plotting the spectrum of the original signal 
plt.figure()
plt.title('Spectrum of original signal')
plt.plot(np.abs(spectrum))
plt.xlabel('frequency [Hz]')
plt.ylabel('amplitude')
plt.show()

freq_response = freqs

#This is using the FFT to compare the results to the original audio compared to using it with the filter.
freq_filtered = spectrum * freq_response

#Plots the spectrum of the filtered frequency 
plt.figure()
plt.title('Frequency-filtered spectrum')
plt.plot(np.abs(freq_filtered), label = "Coefficients: 0.01, Length: 4 ")
plt.xlabel('frequency [Hz]')
plt.ylabel('amplitude')
plt.legend()

plt.show()

filtered_samples = fft.irfft(freq_filtered)
display(Audio(filtered_samples, rate=samplerate))