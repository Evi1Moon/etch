// This file automatically generated by:
//   Apache Etch 1.1.0-incubating (LOCAL-0) / java 1.1.0-incubating (LOCAL-0)
//   Fri Jul 16 12:01:27 CEST 2010
// This file is automatically created and should not be edited!

package etch.cbinding.test;
/*
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */


import org.apache.etch.util.core.Who;
import org.apache.etch.bindings.java.msg.Message;
import org.apache.etch.bindings.java.support.DeliveryService;
import org.apache.etch.bindings.java.support.Pool;
import org.apache.etch.bindings.java.support.StubHelper;
import org.apache.etch.bindings.java.support._Etch_AuthException;
import org.apache.etch.bindings.java.support.StubBase;
import org.apache.etch.bindings.java.support.Pool.PoolRunnable;

/**
 * Message to call translator for helloworld.
 * @param <T> helloworld or a subclass thereof.
 */
@SuppressWarnings("unused")
public class Stubhelloworld<T extends helloworld> extends StubBase<T>
{
	/**
	 * Stub for helloworld.
	 * @param svc the delivery service.
	 * @param obj the implementation of helloworld responsive to requests.
	 * @param queued thread pool used to run AsyncMode.QUEUED methods.
	 * @param free thread pool used to run AsyncMode.FREE methods.
	 */
	public Stubhelloworld( DeliveryService svc, T obj, Pool queued, Pool free )
	{
		super( svc, obj, queued, free );
	}
	
	static
	{
	}

	/**
	 * Method used to force static initialization.
	 */
	public static void init()
	{
		// nothing to do.
	}
	
	static
	{
	}
}
