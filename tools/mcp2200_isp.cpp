#include <unistd.h>
#include <mcp2200_simpleio.h>

using namespace SimpleIO;	

int main(int argc, char** argv) {
	int vendorID=0x04D8;
	int productID=0x00DF;
	
	SimpleIOClass::InitMCP2200(vendorID, productID);

	if(SimpleIOClass::IsConnected()) {
		printf("Launch ISP mode via MCP2200 0x%04X:0x%04X\n",vendorID,productID);
		// io: 00110011, output: 00001100
		SimpleIOClass::ConfigureIoDefaultOutput(0x33,0xc);
		
		SimpleIOClass::ClearPin(3);
		sleep(1);
		SimpleIOClass::ClearPin(2);
		sleep(1);
		SimpleIOClass::SetPin(2);
		sleep(2);
		
	}
	else
	{
		cout << "No device found" << "\n";
	}
	exit(0);
}