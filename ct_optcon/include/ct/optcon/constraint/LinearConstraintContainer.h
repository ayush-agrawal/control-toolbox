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

#include "ConstraintContainerBase.h"

namespace ct {
namespace optcon {


/**
 * @ingroup    Constraint
 *
 * @brief      A base function for linear constraint functions which have a
 *             first derivative
 *
 * * The LinearConstraintBase Class is the base class for defining the
 *   non-linear optimization constraints.
 *
 * @tparam     STATE_DIM  Dimension of the state vector
 * @tparam     CONTROL_DIM  Dimension of the control input vector
 */
template <size_t STATE_DIM, size_t CONTROL_DIM, typename SCALAR = double>
class LinearConstraintContainer : public ConstraintContainerBase<STATE_DIM, CONTROL_DIM, SCALAR>
{
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    typedef core::StateVector<STATE_DIM, SCALAR> state_vector_t;
    typedef core::ControlVector<CONTROL_DIM, SCALAR> input_vector_t;

    typedef LinearConstraintContainer<STATE_DIM, CONTROL_DIM, SCALAR>* LinearConstraintContainer_Raw_Ptr_t;
    typedef Eigen::Matrix<SCALAR, Eigen::Dynamic, 1> VectorXs;
    typedef Eigen::Matrix<SCALAR, Eigen::Dynamic, Eigen::Dynamic> MatrixXs;

    /**
	 * @brief      Default constructor
	 */
    LinearConstraintContainer();


    /**
	 * @brief      Copy constructor
	 *
	 * @param[in]  arg   The object to be copied
	 */
    LinearConstraintContainer(const LinearConstraintContainer& arg);
    /**
	 * @brief      Destructor
	 *
	 */
    virtual ~LinearConstraintContainer();

    /**
	 * Clones the linear constraint class
	 * @return pointer to the clone
	 */
    virtual LinearConstraintContainer_Raw_Ptr_t clone() const = 0;

    /**
	 * @brief      Evaluates the constraint jacobian wrt the state using sparse representation
	 *
	 * @param      jacVec  The sparse jacobian vector
	 * @param      count   The size of jacVec
	 */
    virtual VectorXs jacobianStateSparseIntermediate() = 0;

    /**
	 * @brief      Evaluates the constraint jacobian wrt the state
	 *
	 * @return     The jacobian wrt state
	 */
    virtual MatrixXs jacobianStateIntermediate() = 0;

    /**
	 * @brief      Evaluates the constraint jacobian wrt the state using sparse representation
	 *
	 * @param      jacVec  The sparse jacobian vector
	 * @param      count   The size of jacVec
	 */
    virtual VectorXs jacobianStateSparseTerminal() = 0;

    /**
	 * @brief      Evaluates the constraint jacobian wrt the state
	 *
	 * @return     The jacobian wrt state
	 */
    virtual MatrixXs jacobianStateTerminal() = 0;

    /**
	 * @brief      Evaluates the constraint jacobian wrt the control input using sparse representation
	 *
	 * @param      jacVec  The sparse jacobian vector
	 * @param      count   The size of jacVec
	 */
    virtual VectorXs jacobianInputSparseIntermediate() = 0;

    /**
	 * @brief      Evaluates the constraint jacobian wrt the control input
	 *
	 * @return     The jacobian wrt control
	 */
    virtual MatrixXs jacobianInputIntermediate() = 0;

    /**
	 * @brief      Evaluates the constraint jacobian wrt the control input using sparse representation
	 *
	 * @param      jacVec  The sparse jacobian vector
	 * @param      count   The size of jacVec
	 */
    virtual VectorXs jacobianInputSparseTerminal() = 0;

    /**
	 * @brief      Evaluates the constraint jacobian wrt the control input
	 *
	 * @return     The jacobian wrt control
	 */
    virtual MatrixXs jacobianInputTerminal() = 0;

    /**
	 * @brief      Returns the sparsity pattern for the jacobian wrt state
	 *
	 * @param      iRows  The vector of the row indices containing non zero
	 *                    elements in the constraint jacobian
	 * @param      jCols  The vector of the column indices containing non zero
	 *                    elements in the constraint jacobian
	 *
	 * @return     The size of iRow and jCols
	 */
    virtual void sparsityPatternStateIntermediate(Eigen::VectorXi& iRows, Eigen::VectorXi& jCols) = 0;

    /**
	 * @brief      Returns the sparsity pattern for the jacobian wrt state
	 *
	 * @param      iRows  The vector of the row indices containing non zero
	 *                    elements in the constraint jacobian
	 * @param      jCols  The vector of the column indices containing non zero
	 *                    elements in the constraint jacobian
	 *
	 * @return     The size of iRow and jCols
	 */
    virtual void sparsityPatternStateTerminal(Eigen::VectorXi& iRows, Eigen::VectorXi& jCols) = 0;

    /**
	 * @brief      Returns the sparsity pattern for the jacobian wrt control
	 *
	 * @param      iRows  The vector of the row indices containing non zero
	 *                    elements in the constraint jacobian
	 * @param      jCols  The vector of the column indices containing non zero
	 *                    elements in the constraint jacobian
	 *
	 * @return     The size of iRow and jCols
	 */
    virtual void sparsityPatternInputIntermediate(Eigen::VectorXi& iRows, Eigen::VectorXi& jCols) = 0;

    /**
	 * @brief      Returns the sparsity pattern for the jacobian wrt control
	 *
	 * @param      iRows  The vector of the row indices containing non zero
	 *                    elements in the constraint jacobian
	 * @param      jCols  The vector of the column indices containing non zero
	 *                    elements in the constraint jacobian
	 *
	 * @return     The size of iRow and jCols
	 */
    virtual void sparsityPatternInputTerminal(Eigen::VectorXi& iRows, Eigen::VectorXi& jCols) = 0;

    /**
	 * @brief      Returns the number of non zero elements in the constraint jacobian wrt state
	 *
	 * @return     The number of the non zeros
	 */
    virtual size_t getJacobianStateNonZeroCountIntermediate() = 0;

    /**
	 * @brief      Returns the number of non zero elements in the constraint jacobian wrt state
	 *
	 * @return     The number of the non zeros
	 */
    virtual size_t getJacobianStateNonZeroCountTerminal() = 0;

    /**
	 * @brief      Returns the number of non zero elements in the constraint jacobian wrt input
	 *
	 * @return     The number of the non zeros
	 */
    virtual size_t getJacobianInputNonZeroCountIntermediate() = 0;

    /**
	 * @brief      Returns the number of non zero elements in the constraint jacobian wrt input
	 *
	 * @return     The number of the non zeros
	 */
    virtual size_t getJacobianInputNonZeroCountTerminal() = 0;

    /**
	 * @brief      Returns the number of non zeros in the constraint jacobian
	 *             wrt to state and input
	 *
	 * @return      The number of the non zeros
	 */
    size_t getJacNonZeroCount();
    /**
	 * @brief      Initializes the constraint container
	 */
    void initialize();

    /**
	 * @brief      Initializes the intermediate constraints
	 *
	 * @return     Returns true if the initialization was successful
	 */
    virtual bool initializeIntermediate() = 0;

    /**
	 * @brief      Initializes the terminal constraints
	 *
	 * @return     Returns true if the initialization was successful
	 */
    virtual bool initializeTerminal() = 0;

    /**
	 * @brief      Checks if the constraint container is initialized
	 *
	 * @return     Returns true if initialized
	 */
    bool isInitialized();

protected:
    bool initializedIntermediate_;
    bool initializedTerminal_;
};

}  // namespace optcon
}  // namespace ct
