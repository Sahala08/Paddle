// Copyright (c) 2021 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.1 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.1
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "paddle/fluid/operators/elementwise/elementwise_op_impl.cu.h"
#include "paddle/fluid/operators/kernel_primitives/kernel_primitives.h"

namespace paddle {
namespace operators {

namespace kps = paddle::operators::kernel_primitives;

template <ElementwiseType ET, typename InT, typename OutT, typename Functor,
          int NumOuts = 1>
void LaunchBroadcastElementwiseCudaKernel(
    const platform::CUDADeviceContext &ctx,
    const std::vector<const framework::Tensor *> &ins,
    std::vector<framework::Tensor *> *outs, int axis, Functor func) {
  std::vector<const pten::DenseTensor *> pt_inputs;
  std::vector<pten::DenseTensor *> pt_outputs;
  // TODO(YuanRisheng) *_tmp for cache DenseTensor, because the temporary
  // DenseTensor obj
  // generated by MakePtenDenseTensor can be destroyed when exits loop. *_tmp
  // can be deleted
  // when DenseTensor support copy constructor.
  std::vector<std::unique_ptr<pten::DenseTensor>> pt_inputs_tmp;
  std::vector<std::unique_ptr<pten::DenseTensor>> pt_outputs_tmp;
  for (auto in : ins) {
    pt_inputs_tmp.emplace_back(
        std::move(paddle::experimental::MakePtenDenseTensor(*in)));
  }
  for (auto out : *outs) {
    pt_outputs_tmp.emplace_back(
        std::move(paddle::experimental::MakePtenDenseTensor(*out)));
  }
  for (int i = 0; i < pt_inputs_tmp.size(); i++) {
    pt_inputs.push_back(pt_inputs_tmp[i].get());
  }
  for (int i = 0; i < pt_outputs_tmp.size(); i++) {
    pt_outputs.push_back(pt_outputs_tmp[i].get());
  }
  pten::LaunchBroadcastElementwiseCudaKernel<ET, InT, OutT, Functor, NumOuts>(
      ctx, pt_inputs, &pt_outputs, axis, func);
}

template <ElementwiseType ET, typename InT, typename OutT, typename Functor,
          int NumOuts = 1>
void LaunchElementwiseCudaKernel(
    const platform::CUDADeviceContext &cuda_ctx,
    const std::vector<const framework::Tensor *> &ins,
    std::vector<framework::Tensor *> *outs, int axis, Functor func) {
  std::vector<const pten::DenseTensor *> pt_inputs;
  std::vector<pten::DenseTensor *> pt_outputs;
  // TODO(YuanRisheng) *_tmp for cache DenseTensor, because the temporary
  // DenseTensor obj
  // generated by MakePtenDenseTensor can be destroyed when exits loop. *_tmp
  // can be deleted
  // when DenseTensor support copy constructor.
  std::vector<std::unique_ptr<pten::DenseTensor>> pt_inputs_tmp;
  std::vector<std::unique_ptr<pten::DenseTensor>> pt_outputs_tmp;
  for (auto in : ins) {
    pt_inputs_tmp.emplace_back(
        std::move(paddle::experimental::MakePtenDenseTensor(*in)));
  }
  for (auto out : *outs) {
    pt_outputs_tmp.emplace_back(
        std::move(paddle::experimental::MakePtenDenseTensor(*out)));
  }
  for (int i = 0; i < pt_inputs_tmp.size(); i++) {
    pt_inputs.push_back(pt_inputs_tmp[i].get());
  }
  for (int i = 0; i < pt_outputs_tmp.size(); i++) {
    pt_outputs.push_back(pt_outputs_tmp[i].get());
  }
  pten::LaunchElementwiseCudaKernel<ET, InT, OutT, Functor, NumOuts>(
      cuda_ctx, pt_inputs, &pt_outputs, axis, func);
}

}  // namespace operators
}  // namespace paddle
