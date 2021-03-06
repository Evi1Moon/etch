// This file automatically generated by:
//   Apache Etch 1.1.0-incubating (LOCAL-0) / csharp 1.1.0-incubating (LOCAL-0)
//   Mon Feb 16 11:13:50 CST 2009
// This file is automatically created for your convenience and will not be
// overwritten once it exists! Please edit this file as necessary to implement
// your service logic.

using System;



using org.apache.etch.interoptester.example.iot.types.IOT;

///<summary>Your custom implementation of BaseIOTClient. Add methods here to provide
///implementation of messages from the server. </summary>
namespace org.apache.etch.interoptester.example.iot
{
	///<summary>Implementation for ImplIOTClient</summary>
	public class ImplIOTClient : BaseIOTClient
	{
		/// <summary>Constructs the ImplIOTClient.</summary>
 		/// <param name="server">a connection to the server session. Use this to
 		/// send a message to the server.</param>
		public ImplIOTClient(RemoteIOTServer server)
		{
			this.server = server;
		}
		
		/// <summary>A connection to the server session. Use this to
 		/// send a message to the server.</summary>
		private readonly RemoteIOTServer server;
	
		// TODO: Implement delegates or provide implementation of IOTClient
		// messages from the server	
	}
}