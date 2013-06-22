s = Server.default;
s.boot();

 SynthDef.new("oscSine", {arg freq = 440.0;
  var sin, env_gen, env;
  
  env = Env.triangle(1.0, 0.2);
  env_gen = EnvGen.kr(env);
  sin = SinOsc.ar(freq, mul: env_gen);
  Out.ar(0, sin);
 }).writeDefFile;



x = [
[0, [ \s_new, \oscSine, 1000, 0, 0,  \freq, 27.5 ]],
[1, [ \s_new, \oscSine, 1001, 0, 0,  \freq, 30.8677 ]],
[2, [ \s_new, \oscSine, 1002, 0, 0,  \freq, 32.7032 ]],
[3, [ \s_new, \oscSine, 1003, 0, 0,  \freq, 36.7081 ]],
[4, [ \s_new, \oscSine, 1004, 0, 0,  \freq, 41.2034 ]],
[5, [ \s_new, \oscSine, 1005, 0, 0,  \freq, 43.6535 ]],
[6, [ \s_new, \oscSine, 1006, 0, 0,  \freq, 48.9994 ]],
[7, [ \s_new, \oscSine, 1007, 0, 0,  \freq, 55 ]],
[8, [ \s_new, \oscSine, 1008, 0, 0,  \freq, 61.7354 ]],
[9, [ \s_new, \oscSine, 1009, 0, 0,  \freq, 65.4064 ]],
[10, [ \s_new, \oscSine, 1010, 0, 0,  \freq, 73.4162 ]],
[11, [ \s_new, \oscSine, 1011, 0, 0,  \freq, 82.4069 ]],
[12, [ \s_new, \oscSine, 1012, 0, 0,  \freq, 87.3071 ]],
[13, [ \s_new, \oscSine, 1013, 0, 0,  \freq, 97.9989 ]],
[14, [ \s_new, \oscSine, 1014, 0, 0,  \freq, 110 ]],
[15, [ \s_new, \oscSine, 1015, 0, 0,  \freq, 123.471 ]],
[16, [ \s_new, \oscSine, 1016, 0, 0,  \freq, 130.813 ]],
[17, [ \s_new, \oscSine, 1017, 0, 0,  \freq, 146.832 ]],
[18, [ \s_new, \oscSine, 1018, 0, 0,  \freq, 164.814 ]],
[19, [ \s_new, \oscSine, 1019, 0, 0,  \freq, 174.614 ]],
[20, [ \s_new, \oscSine, 1020, 0, 0,  \freq, 195.998 ]],
[21, [ \s_new, \oscSine, 1021, 0, 0,  \freq, 220 ]],
[22, [ \s_new, \oscSine, 1022, 0, 0,  \freq, 246.942 ]],
[23, [ \s_new, \oscSine, 1023, 0, 0,  \freq, 261.626 ]],
[24, [ \s_new, \oscSine, 1024, 0, 0,  \freq, 293.665 ]],
[25, [ \s_new, \oscSine, 1025, 0, 0,  \freq, 329.628 ]],
[26, [ \s_new, \oscSine, 1026, 0, 0,  \freq, 349.228 ]],
[27, [ \s_new, \oscSine, 1027, 0, 0,  \freq, 391.995 ]],
[28, [ \s_new, \oscSine, 1028, 0, 0,  \freq, 440 ]],
[29, [ \s_new, \oscSine, 1029, 0, 0,  \freq, 493.883 ]],
[30, [ \s_new, \oscSine, 1030, 0, 0,  \freq, 523.251 ]],
[31, [ \s_new, \oscSine, 1031, 0, 0,  \freq, 587.33 ]],
[32, [ \s_new, \oscSine, 1032, 0, 0,  \freq, 659.255 ]],
[33, [ \s_new, \oscSine, 1033, 0, 0,  \freq, 698.456 ]],
[34, [ \s_new, \oscSine, 1034, 0, 0,  \freq, 783.991 ]],
[35, [ \s_new, \oscSine, 1035, 0, 0,  \freq, 880 ]],
[36, [ \s_new, \oscSine, 1036, 0, 0,  \freq, 987.767 ]],
[37, [ \s_new, \oscSine, 1037, 0, 0,  \freq, 1046.5 ]],
[38, [ \s_new, \oscSine, 1038, 0, 0,  \freq, 1174.66 ]],
[39, [ \s_new, \oscSine, 1039, 0, 0,  \freq, 1318.51 ]],
[40, [ \s_new, \oscSine, 1040, 0, 0,  \freq, 1396.91 ]],
[41, [ \s_new, \oscSine, 1041, 0, 0,  \freq, 1567.98 ]],
[42, [ \s_new, \oscSine, 1042, 0, 0,  \freq, 1760 ]],
[43, [ \s_new, \oscSine, 1043, 0, 0,  \freq, 1975.53 ]], 
[44, [ \s_new, \oscSine, 1044, 0, 0,  \freq, 2093 ]],
[45, [ \s_new, \oscSine, 1045, 0, 0,  \freq, 2349.32 ]],
[46, [ \s_new, \oscSine, 1046, 0, 0,  \freq, 2637.02 ]],
[47, [ \s_new, \oscSine, 1047, 0, 0,  \freq, 2793.83 ]],
[48, [ \s_new, \oscSine, 1048, 0, 0,  \freq, 3135.96 ]],
[49, [ \s_new, \oscSine, 1049, 0, 0,  \freq, 3520 ]],
[50, [ \s_new, \oscSine, 1050, 0, 0,  \freq, 3951.07 ]],
[51, [ \s_new, \oscSine, 1051, 0, 0,  \freq, 4186.01 ]],
[52, [\c_set, 0, 0]]
];

Score.write(x, "./risingNotes.osc");



0.exit();

