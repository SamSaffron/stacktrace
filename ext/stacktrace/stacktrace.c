#include <ruby.h>
#include <vm_core.h>
#include <iseq.h>
#ifdef RUBY193
    #define ruby_current_thread ((rb_thread_t *)RTYPEDDATA_DATA(rb_thread_current()))
#endif


static VALUE stacktrace(rb_thread_t *th)
{
  VALUE ary = rb_ary_new();
  
  const rb_control_frame_t *limit_cfp = th->cfp;
  const rb_control_frame_t *cfp = (void *)(th->stack + th->stack_size);
  
  cfp -= 2;
  
  limit_cfp = RUBY_VM_NEXT_CONTROL_FRAME(limit_cfp);

  while (cfp > limit_cfp) {
	  if (cfp->iseq != 0) {
	      if (cfp->pc != 0) {
		      rb_iseq_t *iseq = cfp->iseq;
          if (cfp->me) {
            rb_ary_push(ary, cfp->me->klass);
          } else if (iseq->defined_method_id) {
            VALUE hash = rb_hash_new();
            rb_hash_aset(hash, ID2SYM(rb_intern("klass")), iseq->klass); 
            rb_hash_aset(hash, ID2SYM(rb_intern("method")), iseq->name); 
            rb_ary_push(ary,hash);
          }
	      }
	  }
	  
	  cfp = RUBY_VM_NEXT_CONTROL_FRAME(cfp);
  }
  return rb_ary_reverse(ary);
}

static VALUE rb_st_kernel_stacktrace(void)
{
  return stacktrace(GET_THREAD());
}

static VALUE rb_st_thread_stacktrace(VALUE thval)
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

  return stacktrace(th);
}

void Init_stacktrace()
{
    rb_define_global_function("stacktrace", rb_st_kernel_stacktrace, 0);
    rb_define_method(rb_cThread, "stacktrace", rb_st_thread_stacktrace, 0);
}
