#include <windows.h>
#include <string>
#include <iostream>
#include <winhttp.h>


std::wstring payload = L"guid=implantTest&username=TestUser&=computername=TestComputer\r\n";

std::string payloadrn = "22677569643d696d706c616e745465737426757365726e616d653d5465737455736572263d636f6d70757465726e616d653d54657374436f6d70757465725c725c6e22";
std::string payloadrnNOq= "677569643d696d706c616e745465737426757365726e616d653d5465737455736572263d636f6d70757465726e616d653d54657374436f6d70757465725c725c6e";

std::wstring hexload = L"677569643d696d706c616e745465737426757365726e616d653d5465737455736572263d636f6d70757465726e616d653d54657374436f6d7075746572";

std::string hexload2 = "22677569643d696d706c616e745465737426757365726e616d653d5465737455736572263d636f6d70757465726e616d653d54657374436f6d7075746572220d0a";

//std::string postman = "{\n    \"guid\": \"abcd1235\",\n    \"computerName\": \"Win15\",\n    \"username\": \"User\",\n    \"ip\": \"192.255.68.03\",\n    \"mac\": \"AB:CD:EF:GG\"\n}";

//std::string postmanhex = "227b5c6e202020205c22677569645c223a205c2261626364313233355c222c5c6e202020205c22636f6d70757465724e616d655c223a205c2257696e31355c222c5c6e202020205c22757365726e616d655c223a205c22557365725c222c5c6e202020205c2269705c223a205c223139322e3235352e36382e30335c222c5c6e202020205c226d61635c223a205c2241423a43443a45463a47475c225c6e7d22";
