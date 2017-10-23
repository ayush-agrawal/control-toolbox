/***********************************************************************************
Copyright (c) 2017, Michael Neunert, Markus Giftthaler, Markus Stäuble, Diego Pardo,
Farbod Farshidian. All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of ETH ZURICH nor the names of its contributors may be used
      to endorse or promote products derived from this software without specific
      prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL ETH ZURICH BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************************/

#pragma once

namespace ct {
namespace optcon {

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::TermQuadTracking(const state_matrix_t& Q,
    const control_matrix_t& R,
    const core::InterpolationType& stateSplineType,
    const core::InterpolationType& controlSplineType,
    const bool trackControlTrajectory)
    : Q_(Q),
      R_(R),
      x_traj_ref_(stateSplineType),
      u_traj_ref_(controlSplineType),
      trackControlTrajectory_(trackControlTrajectory)
{
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::TermQuadTracking()
{
    // default values
    Q_.setIdentity();
    R_.setIdentity();
    trackControlTrajectory_ = false;
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::TermQuadTracking(
    const TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>& arg)
    : TermBase<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>(arg),
      Q_(arg.Q_),
      R_(arg.R_),
      x_traj_ref_(arg.x_traj_ref_),
      u_traj_ref_(arg.u_traj_ref_),
      trackControlTrajectory_(arg.trackControlTrajectory_)
{
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::~TermQuadTracking()
{
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>*
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::clone() const
{
    return new TermQuadTracking(*this);
}


template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
void TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::setWeights(const state_matrix_double_t& Q,
    const control_matrix_double_t& R)
{
    Q_ = Q.template cast<SCALAR_EVAL>();
    R_ = R.template cast<SCALAR_EVAL>();
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
void TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::setStateAndControlReference(
    const core::StateTrajectory<STATE_DIM>& xTraj,
    const core::ControlTrajectory<CONTROL_DIM>& uTraj)
{
    x_traj_ref_ = xTraj;
    u_traj_ref_ = uTraj;
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
template <typename SC>
SC TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::evalLocal(const Eigen::Matrix<SC, STATE_DIM, 1>& x,
    const Eigen::Matrix<SC, CONTROL_DIM, 1>& u,
    const SC& t)
{
    Eigen::Matrix<SC, STATE_DIM, 1> xDiff = x - x_traj_ref_.eval((SCALAR_EVAL)t).template cast<SC>();

    Eigen::Matrix<SC, CONTROL_DIM, 1> uDiff;

    if (trackControlTrajectory_)
        uDiff = u - u_traj_ref_.eval((SCALAR_EVAL)t).template cast<SC>();
    else
        uDiff = u;

    return (xDiff.transpose() * Q_.template cast<SC>() * xDiff + uDiff.transpose() * R_.template cast<SC>() * uDiff)(
        0, 0);
}


template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
SCALAR TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::evaluate(
    const Eigen::Matrix<SCALAR, STATE_DIM, 1>& x,
    const Eigen::Matrix<SCALAR, CONTROL_DIM, 1>& u,
    const SCALAR& t)
{
    return evalLocal<SCALAR>(x, u, t);
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
ct::core::StateVector<STATE_DIM, SCALAR_EVAL>
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::stateDerivative(
    const ct::core::StateVector<STATE_DIM, SCALAR_EVAL>& x,
    const ct::core::ControlVector<CONTROL_DIM, SCALAR_EVAL>& u,
    const SCALAR_EVAL& t)
{
    Eigen::Matrix<SCALAR_EVAL, STATE_DIM, 1> xDiff = x - x_traj_ref_.eval(t);

    return xDiff.transpose() * Q_.transpose() + xDiff.transpose() * Q_;
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
typename TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::state_matrix_t
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::stateSecondDerivative(
    const core::StateVector<STATE_DIM, SCALAR_EVAL>& x,
    const core::ControlVector<CONTROL_DIM, SCALAR_EVAL>& u,
    const SCALAR_EVAL& t)
{
    return Q_ + Q_.transpose();
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
core::ControlVector<CONTROL_DIM, SCALAR_EVAL>
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::controlDerivative(
    const core::StateVector<STATE_DIM, SCALAR_EVAL>& x,
    const core::ControlVector<CONTROL_DIM, SCALAR_EVAL>& u,
    const SCALAR_EVAL& t)
{
    Eigen::Matrix<SCALAR_EVAL, CONTROL_DIM, 1> uDiff;

    if (trackControlTrajectory_)
        uDiff = u - u_traj_ref_.eval(t);
    else
        uDiff = u;

    return uDiff.transpose() * R_.transpose() + uDiff.transpose() * R_;
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
typename TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::control_matrix_t
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::controlSecondDerivative(
    const core::StateVector<STATE_DIM, SCALAR_EVAL>& x,
    const core::ControlVector<CONTROL_DIM, SCALAR_EVAL>& u,
    const SCALAR_EVAL& t)
{
    return R_ + R_.transpose();
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
typename TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::control_state_matrix_t
TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::stateControlDerivative(
    const core::StateVector<STATE_DIM, SCALAR_EVAL>& x,
    const core::ControlVector<CONTROL_DIM, SCALAR_EVAL>& u,
    const SCALAR_EVAL& t)
{
    return control_state_matrix_t::Zero();
}

template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR_EVAL, typename SCALAR>
void TermQuadTracking<STATE_DIM, CONTROL_DIM, SCALAR_EVAL, SCALAR>::loadConfigFile(const std::string& filename,
    const std::string& termName,
    bool verbose)
{
    loadMatrixCF(filename, "Q", Q_, termName);
    loadMatrixCF(filename, "R", R_, termName);
    if (verbose)
    {
        std::cout << "Read Q as Q = \n" << Q_ << std::endl;
        std::cout << "Read R as R = \n" << R_ << std::endl;
    }
}
}
}
