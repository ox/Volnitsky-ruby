#include <stdint.h>
#include <algorithm>
#include <limits>

// Include the Ruby headers and goodies
#include "ruby.h"

const char*  volnitsky(const char*  S,  const char*  Se, const char*  SS,  const char*  SSe) {
        // only for little-endian platforms and where access to misaligned W is allowed

                typedef         uint16_t        W_t;
                typedef         uint8_t         offset_t;

        const size_t    W_size   = sizeof(W_t);
        const size_t    S_size   = Se - S;
        const size_t    SS_size  = SSe - SS;
        const size_t    step     = SS_size - W_size + 1;

        // arg sizes check
        if  (
                    SS_size  <  2*W_size-1
                ||  SS_size  >= std::numeric_limits<offset_t>::max()
                ||  S_size   <  2*SS_size
        )
                return  std::search(S,Se,SS,SSe);       // fallback to std::search

        // make hash
        const size_t    H_size     = 64*1024;
        offset_t         H[H_size]  = {0};
        for  (size_t  i=0;   i < SS_size-W_size+1;   i++)  {
                size_t  h  =  *(W_t*) (SS+i)  % H_size;
                while (H[h])    h = (h+1) % H_size;             // find free cell
                H[h] = i+1;
        }

        // step through text
        const char*   P = S + SS_size - W_size;
        for  (;   P <= Se-SS_size;   P += step)  {
                size_t  h   = *(W_t*) P  %  H_size;
                if  (H[h])  {
                        for (;   H[h];   h = (h+1) % H_size )  {
                                const char*    R  =  P - (H[h] - 1);
                                for ( size_t i=0;  i<SS_size;  i++)  {
                                        if  (R[i] != SS[i])   goto next_hash_cell;
                                }
                                return R;                       // found
                                next_hash_cell:;
                        }
                }
        }

        // check tail
        return  std::search(P-step+1,Se,SS,SSe);
};

extern "C"
VALUE search(VALUE klass, VALUE S,VALUE SS) { 
	const char* s= RSTRING_PTR(S); 
	const char* se=s+strlen(s); 
	const char* ss= RSTRING_PTR(SS); 
	const char* sse=ss+strlen(ss);  
	const char* returned = volnitsky(s,se,ss,sse);
	return INT2NUM(strlen(s)-strlen(returned));
}

extern "C"
void Init_volnitsky(){
	VALUE mVolnitsky = rb_define_module("Volnitsky");
	rb_define_method(mVolnitsky, "search", (VALUE(*)(...))search, 2);
}


