#include <u.h>
#include <libc.h>

int ibase = 10;
int obase = 10;

/* use A-F instead of a-f in output */
int upper = 0;

int parsebase(char *s,int b) {
	int n = 0;
	int d; /* digit, parsed */
	int i;
	char c; /* digit, unparsed */
	for (i=0;c=s[i];i++) {
		if (c=='_') continue;
		n *= b;
		if ('0' <= c && c <= '9') {
			d = c - '0';
		} else if ('a' <= c && c <= 'f') {
			d = c - 'a' + 10;
		} else if ('A' <= c && c <= 'F') {
			d = c - 'A' + 10;
		} else {
			sysfatal("unknown digit %c",c);
		}

		if (d < b) {
			n = n + d;
		} else {
			sysfatal("invalid digit %c for base %d", c, b);
		}
	}
	return n;
}

int parsenum(char *s, int b) {
	int o = 0; /* offset, i.e. length of prefix */
	int sg = 1; /* sign */

	if (s[0]==0) sysfatal("empty string");
	
	if (s[0]=='-') {
		sg = -1;
		o = 1;
	}
	/* handle base prefixes */
	if (s[o]=='0') {
		o++;
		switch(s[o]) {
		case 'x':
			o++;
			b = 16;
			break;
		case 'o':
			o++;
			b = 8;
			break;
		case 'b':
			o++;
			b = 2;
			break;
		case 'd':
			o++;
			b = 10;
			break;
		}
	}

	return sg*parsebase(s+o,b);
}

void printnum(int n) {
	/* +1 for null termination */
	char buf[33] = 0;
	int i;
	int d;
	if (n < 0) {
		n = -n;
		write(1,"-",1);
	}
	for (i=0;i<32;i++) {
		d = n%obase;
		n -= d;
		n /= obase;
		if (d < 10) {
			buf[31-i] = '0' + d;
		} else 
			buf[31-i] = d + (upper ? 'A' : 'a') - 10;
		if (n < 1) {
			write(1,buf+31-i,i+2);
			write(1,"\n",1);
			return;
		}
	}
	/* the number will likely overflow before this,
	 * but we don't want to just silently fail */
	sysfatal("cannot print number: too large");
}

void usage(void) {
	sysfatal("usage: %s [ -o obase ] [ -i ibase ] [ -u ] [ n ... ]",argv0);
}
	

void main(int argc,char *argv[]) {
	ARGBEGIN{
	case 'i':
		ibase = parsenum(EARGF(usage()),10);
		break;
	case 'o':
		obase = parsenum(EARGF(usage()),10);
		break;
	case 'u':
		upper = 1;
		break;
	default:
		usage();
	}ARGEND
	
	if (ibase > 16||obase > 16)
		sysfatal("base exeeds maximum of 16");

	int i;
	for (i=0;i<argc;i++) {
		printnum(parsenum(argv[i],ibase));
	}
}