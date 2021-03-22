
template<typename tt>
void Write_to_address(LPVOID adrr, int size, tt  value[]) {

	DWORD oldprotection;
	DWORD garbage;
	
	// changing protection of the address to READ_WRITE in case writting to assembly code 
	VirtualProtect(adrr, sizeof(tt)*size, PAGE_READWRITE, &oldprotection);
	 
	// Writing to memory
	 for (int i = 0 ; i < size ; i++)
		*((tt*)adrr + i) = *(value + i);
	 //
	 printf("patched at 0x%x\n", adrr); 
	
	 // restoring old protection 
	VirtualProtect(adrr, sizeof(tt)*size, oldprotection, &garbage);


}

#define SIZE_OF_FUNCTION 1000 

DWORD H00k_CALL(LPVOID Function_to_Hook, DWORD Code , int callNumber) {

	BYTE *  function_code = (BYTE * )Function_to_Hook;
	DWORD  * Hookat = NULL; //Position where to Hook .
	DWORD   Original_call; 
	int count = 0 ;
	
	for (int i = 0; i < SIZE_OF_FUNCTION ; i++) {
		
		if (count == callNumber) {// Looking for the right CALL to hook . 

			Hookat = (DWORD*)(function_code + i); 
			break; 
		} 
		if (*(function_code + i) == 0xE8) count ++; // Looking for CALL instructions . 

		 }

	if (Hookat == NULL) {
		printf("[x] Failed finding the position to Hook , check your offsets again \n"); 
		exit(1); 
	}
	Original_call = * Hookat ;
	
	
	// Now we have the offset lets hook
	DWORD patc[1] = { Code - (DWORD)Hookat - 4  }; // Near Call storage on memory .

	Write_to_address<DWORD>((BYTE*)Hookat , 1 , patc); 
	
	printf("[+] Hooked!\n"); 
	
	

	return (Original_call+(DWORD)Hookat + 4)  ; // Original CALL to restore after the CODE HOOKING DONE .  

}



