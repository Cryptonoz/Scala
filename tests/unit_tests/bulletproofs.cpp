// Copyright (c) 2017-2018, The Monero Project
// 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "gtest/gtest.h"

#include "ringct/rctOps.h"
#include "ringct/bulletproofs.h"
#include "misc_log_ex.h"

TEST(bulletproofs, valid_zero)
{
  rct::Bulletproof proof = bulletproof_PROVE(0, rct::skGen());
  ASSERT_TRUE(rct::bulletproof_VERIFY(proof));
}

TEST(bulletproofs, valid_max)
{
  rct::Bulletproof proof = bulletproof_PROVE(0xffffffffffffffff, rct::skGen());
  ASSERT_TRUE(rct::bulletproof_VERIFY(proof));
}

TEST(bulletproofs, valid_random)
{
  for (int n = 0; n < 8; ++n)
  {
    rct::Bulletproof proof = bulletproof_PROVE(crypto::rand<uint64_t>(), rct::skGen());
    ASSERT_TRUE(rct::bulletproof_VERIFY(proof));
  }
}

TEST(bulletproofs, valid_multi_random)
{
  for (int n = 0; n < 8; ++n)
  {
    size_t outputs = 2 + n;
    std::vector<uint64_t> amounts;
    rct::keyV gamma;
    for (size_t i = 0; i < outputs; ++i)
    {
      amounts.push_back(crypto::rand<uint64_t>());
      gamma.push_back(rct::skGen());
    }
    rct::Bulletproof proof = bulletproof_PROVE(amounts, gamma);
    ASSERT_TRUE(rct::bulletproof_VERIFY(proof));
  }
}


TEST(bulletproofs, invalid_8)
{
  rct::key invalid_amount = rct::zero();
  invalid_amount[8] = 1;
  rct::Bulletproof proof = bulletproof_PROVE(invalid_amount, rct::skGen());
  ASSERT_FALSE(rct::bulletproof_VERIFY(proof));
}

TEST(bulletproofs, invalid_31)
{
  rct::key invalid_amount = rct::zero();
  invalid_amount[31] = 1;
  rct::Bulletproof proof = bulletproof_PROVE(invalid_amount, rct::skGen());
  ASSERT_FALSE(rct::bulletproof_VERIFY(proof));
}
