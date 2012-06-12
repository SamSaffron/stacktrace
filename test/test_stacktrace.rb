require "test/unit"
require "stacktrace"
require "benchmark"

class Object
  def metaclass; class << self; self; end; end
end

class TestStacktrace < Test::Unit::TestCase
  def test_we_get_an_array
    assert_equal Array, stacktrace.class
  end

  def test_we_get_items_in_our_array
    assert stacktrace.length > 0
  end

  def self.test_singleton
    stacktrace
  end

  def test_singleton_stacktrace
    frame = TestStacktrace.test_singleton[0]
    assert_equal frame.klass, TestStacktrace.metaclass 
    assert_equal frame.method, "test_singleton" 
  end
  
  def test_thread_stacktrace
    frame = Thread.current.stacktrace[0]
    assert_equal frame.klass, TestStacktrace
    assert_equal frame.method, "test_thread_stacktrace"
  end

  def test_c_func
    frame = nil
    [1].map{frame = stacktrace[1]}
    
    assert_equal frame.method, "map"
    assert_equal frame.klass, Array
  end

  def test_filename
    name = stacktrace[0].filename
    assert_equal name, __FILE__
  end

  def test_linenumber 
    line = stacktrace[0].line_number
    assert_equal __LINE__ - 1, line
  end

  def test_skipping
    assert_equal stacktrace(1)[0], stacktrace[(1..-1)][0]
  end

  def test_simple_range
    assert_equal stacktrace[1..2], stacktrace(1,2)
  end

  def test_invalid_range
    assert_equal stacktrace(1,-10000), []
  end

  def test_negative_start
    assert_equal stacktrace(-1), [stacktrace[-1]]
  end

  def test_same_length_as_caller
    # caller skips 1 by default
    caller_length = caller(0).length 
    stack_length = stacktrace.length
    assert_equal caller_length, stack_length
  end

  private 

  # stacktrace is MUCH faster
  #def test_demo_bench
  #  Benchmark.bm(7) do |b|
  #    b.report("caller: ") { 100000.times { caller } }
  #    b.report("stacktrace: ") { 100000.times { stacktrace } }
  #  end
  #end

end
