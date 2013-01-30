// $Id$
// 
// Copyright 2007-2008 Cisco Systems Inc.
// 
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy
// of the License at
//  
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations
// under the License.

using System.Collections.Generic;

namespace Etch.Util
{
    /// <summary>
    /// A hashmap which is from string to integer
    /// </summary>
    public class StrIntHashMap : Dictionary<string, int?>
    {
        public StrIntHashMap()
        {
            // nothing to do .
        }

        /// <summary>
        /// Constructs a StrIntHashMap initialized from another
        /// </summary>
        /// <param name="other"></param>
        public StrIntHashMap( StrIntHashMap other )
            : base( other )
        { }

    }
}