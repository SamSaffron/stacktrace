// much of this is based off reading MRI source code

#include <ruby.h>
#include <vm_core.h>
#include <iseq.h>
#ifdef RUBY193
    #define ruby_current_thread ((rb_thread_t *)RTYPEDDATA_DATA(rb_thread_current()))
#endif


static VALUE stacktrace(int argc, VALUE* argv, rb_thread_t *th)
{
  VALUE ary = rb_ary_new();
  
  int start = 0;
  int finish = -1;
  int stack_size = 0;
  const rb_control_frame_t *limit_cfp = th->cfp;
  const rb_control_frame_t *initial_cfp = th->cfp;
  const rb_control_frame_t *cfp = (void *)(th->stack + th->stack_size);
  VALUE file = Qnil;
  int line = 0;
  
  if (argc > 0) {
    start = NUM2INT(argv[0]);
  }

  if (argc > 1) {
    finish = NUM2INT(argv[1]);
  } 

  cfp -= 2;
  limit_cfp -= 1;
  stack_size = cfp - initial_cfp;

  if (finish < 0) {
    finish += 1;
    finish = stack_size + finish;
  }

  if (start < 0) {
    start += 1; 
    start = stack_size + start; 
  }
  
  
  //if (cfp > (limit_cfp + finish + 1)) {
  //   cfp = initial_cfp + finish + 1; 
  //} else {
  //  return ary; 
  //}
  
  //if (cfp > limit_cfp + start) {
   // limit_cfp = initial_cfp + start;
  //} else {
  //  return ary;
  //}


  while (cfp > limit_cfp) {
    VALUE hash = 0; 
	  if (cfp->iseq != 0) {
	      if (cfp->pc != 0) {
		      rb_iseq_t *iseq = cfp->iseq;
          if (cfp->me) {
            rb_ary_push(ary, cfp->me->klass);
          } else if (iseq->defined_method_id) {
            hash = rb_hash_new();
            line = rb_vm_get_sourceline(cfp);
            rb_hash_aset(hash, ID2SYM(rb_intern("klass")), iseq->klass); 
            rb_hash_aset(hash, ID2SYM(rb_intern("method")), iseq->name); 
            rb_hash_aset(hash, ID2SYM(rb_intern("filename")), iseq->filename); 
            rb_hash_aset(hash, ID2SYM(rb_intern("linenumber")), INT2FIX(line)); 
            rb_ary_push(ary,hash);
          }
	      }
	  }
    else if (RUBYVM_CFUNC_FRAME_P(cfp)) {
         ID id;
         extern VALUE ruby_engine_name;
         if (NIL_P(file)) file = ruby_engine_name;
         if (cfp->me->def)
           id = cfp->me->def->original_id;
         else
           id = cfp->me->called_id;
         if (id != ID_ALLOCATOR) {

            hash = rb_hash_new();
            rb_hash_aset(hash, ID2SYM(rb_intern("klass")), cfp->me->klass); 
            rb_hash_aset(hash, ID2SYM(rb_intern("method")), rb_id2str(id)); 
            rb_ary_push(ary,hash);
           
         } 
    }
	  
	  cfp = RUBY_VM_NEXT_CONTROL_FRAME(cfp);
  }
  return rb_ary_reverse(ary);
}

static VALUE rb_st_kernel_stacktrace(int argc, VALUE* argv)
{
  return stacktrace(argc, argv, GET_THREAD());
}

static VALUE rb_st_thread_stacktrace(int argc, VALUE* argv, VALUE thval)
{
  rb_thread_t *th = (rb_thread_t *)RTYPEDDATA_DATA(thval);

  switch (th->status) {
       case THREAD_RUNNABLE:
       case THREAD_STOPPED:
       case THREAD_STOPPED_FOREVER:
     break;
       case THREAD_TO_KILL:
       case THREAD_KILLED:
     return Qnil;
  }

  return stacktrace(argc, argv, th);
}

void Init_stacktrace()
{
    rb_define_global_function("stacktrace", rb_st_kernel_stacktrace, -1);
    rb_define_method(rb_cThread, "stacktrace", rb_st_thread_stacktrace, -1);
}
