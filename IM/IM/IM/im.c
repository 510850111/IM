





/*

WSADATA wsa_data;
int ret;
unsigned long ul_addr = INADDR_NONE;

if (argc != 2)
{
printf("usage : %s <IPv4 address> \n", argv[0]);
return -1;
}

ret = WSAStartup(MAKEWORD(2, 2), &wsa_data);
if (ret != 0)
{
printf("WSAStartup failed : %d\n", ret);
return -1;
}

ul_addr = inet_addr(argv[1]);
if (ul_addr == INADDR_NONE || ul_addr == INADDR_ANY)
{
printf("inet_addr failed and return error!\n");
WSACleanup();
return -1;
}

printf("inet_addr returned success\n");

WSACleanup();
return 0;

*/