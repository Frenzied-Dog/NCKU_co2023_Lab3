#include "user.h"

/* exit function, it should be called when the programs on the emulator end */
void exit(void) { call_to_emu(0, 0); }
/* associated with I/O interface, it is used to print single char to the stdout */
void putchar(char ch) { call_to_emu(1, ch); }

void puts(char *s_ptr) {
	for (; *s_ptr!='\0'; s_ptr++) putchar(*s_ptr);
}

void putint(int32_t num) {
	if (num==0) {
		putchar('0');
		return;
	}

	int8_t arr[10] = "";
	if (num < 0) {
		putchar('-');
		num*=-1;
	}

	for (int i=0; num!=0; num/=10) arr[i++] = (num%10)+'0';

	for (int i=9; i>=0; i--) {
		if (arr[i] == 0) continue;
		putchar(arr[i]);
	}
}

void puthex_L(uint32_t num) {
	int8_t arr[8] = "";
	uint32_t mask=0b1111, tmp=0;
	for (int i=0; i<8; i++) {
		tmp = (num & (mask << (i*4))) >> (4*i);
		arr[8-i-1] = (tmp<10 ? tmp+'0' : tmp-10+'a');
	}

	uint8_t first = 1;
	for (int i=0; i<8; i++) {
		if (first && arr[i] == '0') continue;
		first = 0;
		putchar(arr[i]);
	}
}

void puthex_U(uint32_t num) {
	int8_t arr[8] = "";
	uint32_t mask=0b1111, tmp=0;
	for (int i=0; i<8; i++) {
		tmp = (num & (mask << (i*4))) >> (4*i);
		arr[8-i-1] = (tmp<10 ? tmp+'0' : tmp-10+'A');
	}

	uint8_t first = 1;
	for (int i=0; i<8; i++) {
		if (first && arr[i] == '0') continue;
		first = 0;
		putchar(arr[i]);
	}	
}

void printf(char* format, ...) {
	va_list ap;
	va_start(ap, format);
	for (;*format!='\0'; format++) {
		if (*format == '%') {
			format++;
			if (*format == 's')
				puts(va_arg(ap, char*));
			else if (*format == 'c')
				putchar(va_arg(ap, int32_t));
			else if (*format == 'd')
				putint(va_arg(ap, int32_t));
			else if (*format == 'x')
				puthex_L(va_arg(ap,uint32_t));
			else if (*format == '#' && *(format+1) == 'x') {
				format++;
				puts("0x");
				puthex_L(va_arg(ap,uint32_t));
			} else if (*format == 'X')
				puthex_U(va_arg(ap,uint32_t));
			else if (*format == '#' && *(format+1) == 'X') {
				format++;
				puts("0X");
				puthex_U(va_arg(ap,uint32_t));
			} else if (*format == '%')
				putchar('%');
			else format--;
		} else putchar(*format);
	}
	va_end(ap);
}