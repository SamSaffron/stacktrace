class StackFrame
  module Flags 
    METHOD = 1
    KLASS = 2
    FILENAME = 4
    LINENUMBER = 8 
    ALL = 15
  end

  attr_accessor :line_number, :method, :klass, :filename  

  def ==(other)
    line_number == other.line_number && 
      self.method == other.method &&
      klass == other.klass && 
      filename == other.filename
  end
end
