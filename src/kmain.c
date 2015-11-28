
void kmain()
{
	unsigned char *video = 0xb8000;
	video[0] = 'Y';
	video[1] = 0x08;
	video[2] = 'A';
	video[3] = 0x09;
	video[4] = 'U';
	video[5] = 0x0A;
	video[6] = 'K';
	video[7] = 0x0B;
	while(1);
}
