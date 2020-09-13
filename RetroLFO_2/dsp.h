#ifndef DSP_H // include guard
#define DSP_H

typedef int sample;


struct SinOsc {
  private:
    float phi;
  public:
    SinOsc(float _freq,int _sampleRate):phi(_freq/_sampleRate){};
    sample operator()(int t){
      return 128*cos(2*phi*t);
    };
    static SinOsc make(float freq, float sampleRate){
      return SinOsc{freq,sampleRate};
    };
};

template<typename T>
struct FmOsc {
  private:
    T modulator;
  public:
    FmOsc(float _freq, float _sampleRate, int _index, T _modulator)
      :phi(_freq/_sampleRate),index(_index),modulator(_modulator){}
    float phi;
    int index;
    sample operator()(int t){
      return cos(2*phi*t+index*modulator(t));
    };
    static FmOsc make(float freq, float sampleRate, T fm, int index){
      return FmOsc{freq, sampleRate, index,fm};
    };
};

template<typename T>
struct GainN {
  private: 
    T inputNode;
  public:
    float gain;
    GainN(T _inputNode,float _gain):inputNode(_inputNode),gain(_gain){}
    sample operator()(int t){
      return gain*inputNode(t);
    };
    static GainN make(float gain, T inputNode){
      return GainN{inputNode, gain};
    };
};

template<typename S1, typename S2>
struct Sum{
  private: 
    S1 signal1;
    S2 signal2;
  public:
    Sum(S1 _signal1,S2 _signal2):signal1(_signal1),signal2(_signal2){}
    sample operator()(int t){
      return signal1(t)+signal2(t);
    };
    static Sum make(S1 s1, S2 s2){
      return Sum{s1,s2};
    };
};

template<typename S1, typename S2>
struct Mult{
  private:
    S1 signal1;
    S2 signal2;
  public:
    Mult(S1 _signal1,S2 _signal2):signal1(_signal1),signal2(_signal2){}
    sample operator()(int t){
      return signal1(t)*signal2(t);
    };
    static Mult make(S1 s1, S2 s2){
      return Mult{s1,s2};
    };
};

template<typename S1, typename S2>
struct Div{
  private:
    const S1 signal1;
    const S2 signal2;
  public:
    Div(S1 _signal1,S2 _signal2):signal1(_signal1),signal2(_signal2){}
    sample operator()(int t){
      return signal1(t)/signal2(t);
    };
    static Div make(S1 s1, S2 s2){
      return Div{s1,s2};
    };
};

template<typename S>
struct Delay{
  private:
    const S input;
    const int delayInSamples;
  public:
    Delay(S _input, int _delayInSamples):input(_input),delayInSamples(_delayInSamples){}
    sample operator()(int t){
      return input(t-delayInSamples);
    };
    static Delay make(S in, int delayTimeInSamps){
      return Delay{ in, delayTimeInSamps};
    }
};

template<typename S>
struct FIR4{
  private:
    const S input;
  public:
    const float c0;
    const float c1;
    const float c2;
    const float c3;
    
    FIR4(float _c0, float _c1, float _c2, float _c3, S _input)
      :c0(_c0),c1(_c1),c2(_c2),c3(_c3),input(_input){}
  
    sample operator()(int t){
      return c0*input(t)+c1*input(t-1)+c2*input(t-2)+c3*input(t-3);
    };
    static FIR4 make(S in, float c0, float c1, float c2, float c3){
      return FIR4{in,c0,c1,c2,c3};
    }
};

#endif /* DSP_H */
