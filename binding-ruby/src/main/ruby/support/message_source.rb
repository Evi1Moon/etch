# package 'etch/bindings/ruby/support'

# A message source is used to model the origin of a message to a
# stub so that a reply might be sent.
#
module MessageSource

  # Sends a message to a message source. The message is encoded
  # in some way (e.g., compressed binary format or xml), and then delivered
  # to a transport (e.g., packetized data stream or http response).
  # 
  # @param recipient a transport specific opaque value which identifies
  # the sender of a message. Delivered by a message handler to the stub
  # and may be passed here to send a message back to the original sender.
  # Passing null means "use the default recipient".
  # 
  # @param msg the message to send.
	
  def message( recipient, msg )
		raise "subclasser responsibility"
	end
  
end
