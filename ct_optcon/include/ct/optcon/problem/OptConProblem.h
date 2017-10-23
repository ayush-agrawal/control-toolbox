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

#include <ct/optcon/constraint/LinearConstraintContainer.h>
#include <ct/optcon/costfunction/CostFunctionQuadratic.hpp>

namespace ct {
namespace optcon {


/*!
 * \defgroup OptConProblem OptConProblem
 *
 * \brief Class that defines how to set up an Optimal Control Problem
 *
 * An finite-horizon optimal control problem is generally defined through
 * 	- nonlinear system dynamics
 * 	- cost function (intermediate + terminal cost)
 * 	- initial state
 * 	- state-input constraints
 * 	- pure state intermediate constraints
 * 	- pure state terminal constraints
 * 	- an overall time horizon
 *
 * 	Note that in most cases, the user can also provide a pointer to the linearized system dynamics. This is optional, and
 * 	in case it is not provided, numerical differentiation will be applied to approximate the linearized dynamics.
 * 	\warning Using numerical differentiation is inefficient and typically slow.
 *
*/
template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR = double>
class OptConProblem
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    static const size_t STATE_D = STATE_DIM;
    static const size_t CONTROL_D = CONTROL_DIM;

    // typedefs
    typedef ct::core::StateVector<STATE_DIM, SCALAR> state_vector_t;
    typedef std::shared_ptr<core::ControlledSystem<STATE_DIM, CONTROL_DIM, SCALAR>> DynamicsPtr_t;
    typedef std::shared_ptr<core::LinearSystem<STATE_DIM, CONTROL_DIM, SCALAR>> LinearPtr_t;
    typedef std::shared_ptr<optcon::CostFunctionQuadratic<STATE_DIM, CONTROL_DIM, SCALAR>> CostFunctionPtr_t;
    typedef std::shared_ptr<optcon::LinearConstraintContainer<STATE_DIM, CONTROL_DIM, SCALAR>> ConstraintPtr_t;

    OptConProblem();


    //! Construct a simple unconstrained Optimal Control Problem
    /*!
	 * Constructor for the simplest problem setup
	 * \warning time and initial state to be specified later
	 *
	 * @param nonlinDynamics the nonlinear system dynamics
	 * @param costFunction a quadratic cost function
	 * @param linearSystem (optional) the linear system holding the dynamics derivatives. If the
	 * user does not specify the derivatives, they are generated automatically using numerical differentiation. Warning: this is slow
	 */
    OptConProblem(DynamicsPtr_t nonlinDynamics, CostFunctionPtr_t costFunction, LinearPtr_t linearSystem = nullptr);


    //! Construct a simple unconstrained optimal control problem, with initial state and final time as constructor arguments
    /*!
	 * @param tf The optimal control problem final time horizon
	 * @param x0 The initial system state
	 * @param nonlinDynamics The nonlinear system dynamics
	 * @param costFunction A quadratic cost function
	 * @param linearSystem (optional) Linearized System Dynamics.
	 */
    OptConProblem(const SCALAR& tf,
        const state_vector_t& x0,
        DynamicsPtr_t nonlinDynamics,
        CostFunctionPtr_t costFunction,
        LinearPtr_t linearSystem = nullptr);


    //! check if all the ingredients for an unconstrained otpimal control problem are there
    void verify() const;

    /*!
	 * returns a pointer to the controlled system
	 * */
    const DynamicsPtr_t getNonlinearSystem() const;

    /*!
	 * returns a pointer to the linear system approximation
	 * */
    const LinearPtr_t getLinearSystem() const;

    /*!
	 * returns a pinter to the cost function
	 * */
    const CostFunctionPtr_t getCostFunction() const;

    /*!
	 * returns a pointer to the controlled system
	 * */
    void setNonlinearSystem(const DynamicsPtr_t dyn);

    /*!
	 * returns a pointer to the linear system approximation
	 * */
    void setLinearSystem(const LinearPtr_t lin);

    /*!
	 * returns a pinter to the cost function
	 * */
    void setCostFunction(const CostFunctionPtr_t cost);

    /*!
	 * set intermediate constraints
	 * @param constraint pointer to intermediate constraint
	 */
    void setStateInputConstraints(const ConstraintPtr_t constraint);

    /*!
	 * set final constraints
	 * @param constraint pointer to a final constraint
	 */
    void setPureStateConstraints(const ConstraintPtr_t constraint);

    /**
	 * @brief      Retrieve the state input constraints
	 *
	 * @return     The state input constraints.
	 */
    const ConstraintPtr_t getStateInputConstraints() const;

    /**
	 * @brief      Retrieves the pure state constraints
	 *
	 * @return     The pure state constraints
	 */
    const ConstraintPtr_t getPureStateConstraints() const;

    /*!
	 * get initial state (called by solvers)
	 * */
    const state_vector_t getInitialState() const;

    /*!
	 * set initial state for first subsystem
	 * */
    void setInitialState(const state_vector_t& x0);

    /*!
	 * get the current time horizon
	 * @return	Time Horizon
	 */
    const SCALAR& getTimeHorizon() const;

    /*!
	 * Update the current time horizon in the Opt.Control Problem (required for example for replanning)
	 * @param tf new time horizon
	 */
    void setTimeHorizon(const SCALAR& tf);


private:
    SCALAR tf_;  //! end time

    state_vector_t x0_;  //! initial state

    DynamicsPtr_t controlledSystem_;  //! the nonlinear system
    CostFunctionPtr_t costFunction_;  //! a quadratic cost function
    LinearPtr_t linearizedSystem_;    //! the linear approximation of the nonlinear system

    ConstraintPtr_t stateInputConstraints_;  //! container of all the intermediate constraints of the problem
    ConstraintPtr_t pureStateConstraints_;   //! container of all the terminal constraints of the problem
};
}
}
