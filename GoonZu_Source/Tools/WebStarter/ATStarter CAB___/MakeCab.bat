CABARC -s 6144 n ATStarter.cab ATStarter.dll ATStarter.inf 
SIGNCODE -spc mycert.spc -v mykey.pvk -n ATStarter -t http://timestamp.verisign.com/scripts/timestamp.dll ATStarter.cab
CheckTrust ATStarter.cab