void noteSendAll(char cmd, char data1, char data2)   //MIDI protocol message, same command for the three channels
{
  for(int u=0;u<=2;u++)
  {
    mididin.write(cmd+u);
    mididin.write(data1);
    mididin.write(data2);
  }
}
