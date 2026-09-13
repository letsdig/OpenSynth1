# OpenSynth1 Documentation & Manual

**OpenSynth1** is an open-source virtual analog synthesizer plugin (VST3) and standalone application inspired by the workflow, architecture, and sound design of Ichiro Toda's classic **Synth1** (and the Clavia Nord Lead 2).

Since OpenSynth1 follows the parameter layout, dual-oscillator FM/sync routing, multi-mode filter, dual LFOs, and arpeggiator structure of Synth1, the classic Synth1 documentation and user guides serve as the primary reference for understanding sound design and parameter behavior.

---

## 📖 Official & Community Manuals

* **[Synth1 Complete Illustrated User Manual (PDF)](https://sound.eti.pg.gda.pl/student/eim/doc/Synth1.pdf)**  
  *Compiled by Zoran Nikolic (v1.12)*.  
  The definitive 40+ page comprehensive illustrated manual covering all synthesizer sections, parameter ranges, modulation sources, arpeggiator settings, and tips.

* **[Synth1 Interactive Web Manual (Robert Heaton)](https://robertheaton.com/2019/04/21/synth1-unofficial-manual/)**  
  An interactive, easy-to-read online breakdown of how each knob and section of the synthesizer works.

* **[Internet Archive PDF Mirror](https://archive.org/download/manualzilla-id-5946989/5946989.pdf)**  
  Permanent preservation mirror of the complete Zoran Nikolic PDF manual.

---

## 🎹 Quick Architecture Overview

If you are already familiar with subtractive synthesis, here is how OpenSynth1's engine is structured:

1. **Oscillator 1**:
   - Waveforms: Sine, Saw, Pulse (with variable pulse width), Triangle.
   - Features Sub-Oscillator (1 or 2 octaves below) and FM depth (frequency modulated by Oscillator 2).
2. **Oscillator 2**:
   - Waveforms: Saw, Pulse, Triangle, Noise.
   - Semi-tone pitch adjustment (-24 to +24) and fine-tuning.
   - Hard Sync to Oscillator 1 and Ring Modulation (AM).
3. **Filter**:
   - Modes: 24 dB/oct Low Pass, 12 dB/oct Low Pass, 12 dB/oct Band Pass, 12 dB/oct High Pass.
   - Controls: Cutoff frequency, Resonance, Saturation (drive), and Keyboard tracking.
   - Dedicated Filter ADSR envelope with bipolar depth (-64 to +63).
4. **Amplifier & Envelopes**:
   - Dedicated Amp ADSR envelope, master volume, and velocity sensitivity.
5. **Modulation (LFO 1 & LFO 2)**:
   - Waveforms: Triangle, Saw Down, Square, Random (Sample & Hold).
   - Tempo sync to host DAW or free-running frequency in Hz.
   - Destination routing: Pitch, Filter Cutoff, Osc 2 Pitch, Pan, and Amp.
6. **Arpeggiator & Effects**:
   - Multi-pattern arpeggiator (Up, Down, Up/Down, Random).
   - Built-in stereo chorus/flanger, tempo-synced stereo delay, distortion/decimator, and equalizer.
