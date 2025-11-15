int argPathFn(const char* szArgLn, char* szPathFn)
{
  char sLine[ILINE];
  char *psL= &sLine[0];
  int iRC;

  if (*szArgLn == ' ') {
    szArgLn++;
  }
  if((iRC= strlen(szArgLn)) > 0) {
    if (strncmp(szArgLn, "/", 1) == 0){
      strcpy(psL, szArgLn);
    } else
    if ((*szArgLn == '.')&&(*(szArgLn+1) == '.')) {
      strcpy(psL, sPath);
      *strrchr(psL, '/') = 0x00;  // find last '/' and substitude to EoS
      if (strlen(psL) <= 1) {
        strcpy(psL, "/");
      }
    } else
    if ((*szArgLn == '.')&&(*(szArgLn+1) == '/')) {
      strcpy(psL, sPath);
      strncat(psL, "/", ILINE);
      strncat(psL, szArgLn +2, ILINE);
    } else {  
      strcpy(psL, sPath);
      if (*(psL+strlen(psL)-1) != '/' ) {
        strncat(psL, "/", ILINE);
      }
      strncat(psL, szArgLn, ILINE);    
    }
  } else {
    strcpy(psL, sPath);
    if (*(psL+strlen(psL)-1) != '/' ) {
      strncat(psL, "/", ILINE);
    }
    iRC= 0;
  }
  // Serial.println(psL);
  strcpy(szPathFn, psL);
  return(iRC);
} /* end of function */