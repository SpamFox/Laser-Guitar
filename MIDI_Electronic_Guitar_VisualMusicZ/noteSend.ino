void noteSend(char cmd, char data1, char data2)   //MIDI protocol message
{
  mididin.write(cmd);
  mididin.write(data1);
  mididin.write(data2);
}
