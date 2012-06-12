// much of this is based off reading MRI source code

#include <ruby.h>
#include <vm_core.h>
#include <iseq.h>
#ifdef RUBY193
    #define ruby_current_thread ((rb_thread_t *)RTYPEDDATA_DATA(rb_thread_current()))
#endif

#define ST_F_METHOD     1 
#define ST_F_KLASS      2 
#define ST_F_FILENAME   4 
#define ST_F_LINENUMBER 8 
#define ST_F_ALL        15 

VALUE c_StackFrame;

static VALUE stacktrace(int argc, VALUE* argv, rb_thread_t *th)
{
  VALUE ary = rb_ary_new();
  
  int start = 0;
  int finish = -1;
  int stack_size = 0;
  const rb_control_frame_t *cfp = th->cfp;
  const rb_control_frame_t *tcfp;
  const rb_control_frame_t *limit_cfp = (void *)(th->stack + th->stack_size);
  VALUE file = Qnil;
  int line = 0;
  rb_iseq_t *iseq = 0;
  ID id;
  VALUE frame;
  extern VALUE ruby_engine_name;
  int flags = ST_F_ALL;
  
  if (argc > 0) {
    start = NUM2INT(argv[0]);
  }

  if (argc > 1) {
    finish = NUM2INT(argv[1]);
  } 

  if (argc > 2) {
    flags = NUM2INT(argv[2]);
  }

  cfp += 1;
  limit_cfp -= 2;

  if (finish > 0) {
    finish--;
  }

  if (start < 0 || finish < 0) {
    tcfp = cfp; 
    while (tcfp < limit_cfp) 
    {
	    if (tcfp->iseq != 0 && cfp->pc != 0) {
        stack_size++;
      }
      else if (RUBYVM_CFUNC_FRAME_P(tcfp)) {
        stack_size++;
      }
      tcfp++;
    }

    if (start < 0) {
      start = stack_size + start;
    }
    if (finish < 0) {
      finish = stack_size + finish;
    }
  }

   // rb_warn("flags: %i", flags & ST_F_KLASS);
    // rb_warn("test %i %i cfp: %i lcfp %i ss %i", start, finish, cfp, limit_cfp, stack_size);

  while (cfp < limit_cfp) {
    VALUE hash = 0; 
	  if (cfp->iseq != 0 && cfp->pc != 0) {
        if (start-- > 0) {cfp++; continue;}
        if (finish-- < 0) break;
        iseq = cfp->iseq;
        frame = rb_class_new_instance(0, 0, c_StackFrame); 
        if (iseq->defined_method_id && ((flags & ST_F_KLASS) == ST_F_KLASS)) {
          rb_iv_set(frame, "@klass", iseq->klass);
        }
        if ((flags & ST_F_METHOD) == ST_F_METHOD) {
          rb_iv_set(frame, "@method", iseq->name);
        }
        if ((flags & ST_F_FILENAME) == ST_F_FILENAME) {
          rb_iv_set(frame, "@filename", iseq->filename);
        }
        if ((flags & ST_F_LINENUMBER) == ST_F_LINENUMBER) {
          line = rb_vm_get_sourceline(cfp);
          rb_iv_set(frame, "@line_number", INT2FIX(line));
        }
        rb_ary_push(ary, frame);
	  }
    else if (RUBYVM_CFUNC_FRAME_P(cfp)) {
         if (start-- > 0) {cfp++; continue;}
         if (finish-- < 0) break;
         if (NIL_P(file)) file = ruby_engine_name;
         if (cfp->me->def)
           id = cfp->me->def->original_id;
         else
           id = cfp->me->called_id;
         if (id != ID_ALLOCATOR) {
            frame = rb_class_new_instance(0, 0, c_StackFrame); 
            if ((flags & ST_F_KLASS) == ST_F_KLASS) {
              rb_iv_set(frame, "@klass", cfp->me->klass);
            }
            if ((flags & ST_F_METHOD) == ST_F_METHOD) {
              rb_iv_set(frame, "@method", rb_id2str(id)); 
            }
            rb_ary_push(ary,frame);
           
         } 
    }
	  cfp += 1;
  }
  return ary;
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
    c_StackFrame = rb_eval_string("StackFrame");
}
