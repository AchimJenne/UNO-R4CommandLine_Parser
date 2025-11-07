int argPathFn(const char* szArgLn, char* szPathFn)
{
  char sLine[ILINE];
  char *psL= &sLine[0];
  int iRC;

  if((iRC= strlen(szArgLn)) > 1) {
    if (!strcmp(szArgLn, " /")){
       strcpy(sLine, "/");
    } else
    if (strstr(szArgLn, " ..")) {
      strcpy(sLine, sPath);
      psL= strrchr(sLine, '/');
      *psL = '\0';
      if (strlen(sLine) <= 1) {
        strcpy(sLine,"/");
      }
    } else
    if (strstr(szArgLn, " .")) {
      strcpy(sLine, sPath);
      strcat(sLine,"/");
      strcat(sLine, szArgLn + 2);
    } else {  
      strcpy(sLine, sPath);
      if (strcmp(sLine, "/")) {
        strcat(sLine,"/");
      }
      strcat(sLine, szArgLn + 1);
    }
  } else {
    strcpy(sLine, sPath);
    if (strcmp(sLine, "/")) {
      strcat(sLine,"/");
    }
    iRC= 0;
  }
  strcpy(szPathFn, psL);
  return(iRC);
} /* end of function */