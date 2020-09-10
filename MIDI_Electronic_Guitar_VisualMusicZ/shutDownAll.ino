void shutDownAll()    //Short command for shutting down all notes on the three channels, prevents some errors
                      //like stuck notes when sliding on the analog strings
{
  for(int y=0;y<24;y++)
  {
    noteSend(128, C1[y], 00);
    noteSend(128, C2[y], 00);
    noteSend(128, C3[y], 00);
    noteSend(129, C1[y], 00);
    noteSend(129, C2[y], 00);
    noteSend(129, C3[y], 00);
    noteSend(130, C1[y], 00);
    noteSend(130, C2[y], 00);
    noteSend(130, C3[y], 00);
  }
}
