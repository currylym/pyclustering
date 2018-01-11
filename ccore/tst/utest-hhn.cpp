/**
*
* Copyright (C) 2014-2018    Andrei Novikov (pyclustering@yandex.ru)
*
* GNU_PUBLIC_LICENSE
*   pyclustering is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   pyclustering is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/


#include "gtest/gtest.h"

#include "utenv_check.hpp"

#include "container/ensemble_data.hpp"

#include "nnet/dynamic_analyser.hpp"
#include "nnet/hhn.hpp"


using namespace ccore::differential;
using namespace ccore::nnet;


static void template_create_delete(const std::size_t p_num_osc) {
    hnn_parameters parameters;
    hhn_network * network = new hhn_network(p_num_osc, parameters);

    ASSERT_EQ(p_num_osc, network->size());

    delete network;
}


TEST(utest_hhn, create_10_oscillators) {
    template_create_delete(10);
}

TEST(utest_hhn, create_20_oscillators) {
    template_create_delete(20);
}

TEST(utest_hhn, create_200_oscillators) {
    template_create_delete(200);
}



static void template_collect_dynamic(const std::size_t p_num_osc,
                                     const std::size_t p_steps,
                                     const hhn_stimulus & p_stimulus,
                                     const std::vector<hhn_dynamic::collect> & collect) {
    hnn_parameters parameters;
    hhn_network network(p_num_osc, parameters);

    hhn_dynamic output_dynamic;
    output_dynamic.disable_all();
    for (auto & data_type : collect) {
        output_dynamic.enable(data_type);
    }

    /* check collected elements */
    std::set<hhn_dynamic::collect> collected_types;
    output_dynamic.get_enabled(collected_types);

    std::set<hhn_dynamic::collect> not_collected_types;
    output_dynamic.get_disabled(not_collected_types);

    ASSERT_EQ(collect.size(), collected_types.size());
    ASSERT_EQ(4 - collect.size(), not_collected_types.size());

    /* simulate and check collected outputs */
    network.simulate(p_steps, 10, solve_type::RUNGE_KUTTA_4, p_stimulus, output_dynamic);

    for (auto & data_type : collected_types) {
        ASSERT_EQ(p_steps + 1, output_dynamic.get_peripheral_dynamic()->at(data_type).size());
    }

    for (auto & data_type : not_collected_types) {
        ASSERT_EQ(0, output_dynamic.get_central_dynamic()->at(data_type).size());
    }
}


TEST(utest_hhn, collect_membran_size_1_steps_10_unstimulated) {
    hhn_stimulus stimulus({ 0 });
    template_collect_dynamic(1, 10, stimulus, { hhn_dynamic::collect::MEMBRANE_POTENTIAL });
}

TEST(utest_hhn, collect_membran_size_1_steps_10_stimulated) {
    hhn_stimulus stimulus({ 1 });
    template_collect_dynamic(1, 10, stimulus, { hhn_dynamic::collect::MEMBRANE_POTENTIAL });
}

TEST(utest_hhn, collect_active_potassium) {
    hhn_stimulus stimulus({ 1 });
    template_collect_dynamic(1, 10, stimulus, { hhn_dynamic::collect::ACTIVE_COND_POTASSIUM });
}

TEST(utest_hhn, collect_active_sodium) {
    hhn_stimulus stimulus({ 1 });
    template_collect_dynamic(1, 10, stimulus, { hhn_dynamic::collect::ACTIVE_COND_SODIUM });
}

TEST(utest_hhn, collect_inactive_sodium) {
    hhn_stimulus stimulus({ 1 });
    template_collect_dynamic(1, 10, stimulus, { hhn_dynamic::collect::INACTIVE_COND_SODIUM });
}

TEST(utest_hhn, collect_all_parameters) {
    hhn_stimulus stimulus({ 1 });
    template_collect_dynamic(1, 10, stimulus, { hhn_dynamic::collect::MEMBRANE_POTENTIAL,
                                                hhn_dynamic::collect::ACTIVE_COND_SODIUM,
                                                hhn_dynamic::collect::INACTIVE_COND_SODIUM,
                                                hhn_dynamic::collect::ACTIVE_COND_POTASSIUM });
}

TEST(utest_hhn, collect_membran_size_1_steps_50) {
    hhn_stimulus stimulus({ 1 });
    template_collect_dynamic(1, 50, stimulus, { hhn_dynamic::collect::MEMBRANE_POTENTIAL });
}

TEST(utest_hhn, collect_membran_size_1_steps_200) {
    hhn_stimulus stimulus({ 1 });
    template_collect_dynamic(1, 200, stimulus, { hhn_dynamic::collect::MEMBRANE_POTENTIAL });
}


TEST(utest_hhn, collect_membran_size_5_steps_20) {
    hhn_stimulus stimulus({ 10, 10, 10, 50, 50 });
    template_collect_dynamic(5, 20, stimulus, { hhn_dynamic::collect::MEMBRANE_POTENTIAL });
}

TEST(utest_hhn, collect_membran_size_5_steps_50) {
    hhn_stimulus stimulus({ 50, 50, 0, 10, 10 });
    template_collect_dynamic(5, 50, stimulus, { hhn_dynamic::collect::MEMBRANE_POTENTIAL });
}



static void template_ensemble_generation(const std::size_t p_num_osc,
                                         const std::size_t p_steps,
                                         const std::size_t p_time,
                                         const hhn_stimulus & p_stimulus,
                                         basic_ensemble_data & p_expected_ensembles,
                                         basic_ensemble & p_expected_dead_neurons) {
    hnn_parameters parameters;
    hhn_network network(p_num_osc, parameters);

    hhn_dynamic output_dynamic;
    output_dynamic.enable(hhn_dynamic::collect::MEMBRANE_POTENTIAL);

    /* simulate and check collected outputs */
    network.simulate(p_steps, p_time, solve_type::RUNGE_KUTTA_4, p_stimulus, output_dynamic);

    basic_ensemble_data   ensembles;
    basic_ensemble        dead_neurons;

    hhn_dynamic::evolution_dynamic & membrane_dynamic = output_dynamic.get_peripheral_dynamic(hhn_dynamic::collect::MEMBRANE_POTENTIAL);
    dynamic_analyser(0.0).allocate_sync_ensembles(membrane_dynamic, ensembles, dead_neurons);

    ASSERT_SYNC_ENSEMBLES(ensembles, p_expected_ensembles, dead_neurons, p_expected_dead_neurons);
}

TEST(utest_hhn, one_without_stimulation) {
    basic_ensemble_data expected_ensembles = { };
    basic_ensemble      dead_neurons = { 0 };

    template_ensemble_generation(1, 400, 100, { 0 }, expected_ensembles, dead_neurons);
}

TEST(utest_hhn, one_with_stimulation) {
    basic_ensemble_data expected_ensembles = { { 0 } };
    basic_ensemble      dead_neurons = { };

    template_ensemble_generation(1, 400, 100, { 25 }, expected_ensembles, dead_neurons);
}

TEST(utest_hhn, global_sync) {
    basic_ensemble_data expected_ensembles = { { 0, 1, 2 } };
    basic_ensemble      dead_neurons = { };

    template_ensemble_generation(3, 300, 50, { 27, 27, 27 }, expected_ensembles, dead_neurons);
}

TEST(utest_hhn, without_stimulation) {
    basic_ensemble_data expected_ensembles = { };
    basic_ensemble      dead_neurons = { 0, 1, 2 };

    template_ensemble_generation(3, 400, 100, { 0, 0, 0 }, expected_ensembles, dead_neurons);
}

TEST(utest_hhn, one_with_one_without_stimulation) {
    basic_ensemble_data expected_ensembles = { { 1 } };
    basic_ensemble      dead_neurons = { 0 };

    template_ensemble_generation(2, 400, 100, { 0, 25 }, expected_ensembles, dead_neurons);
}

TEST(utest_hhn, two_sync_ensembles) {
    basic_ensemble_data expected_ensembles = { { 0, 1 }, { 2, 3 } };
    basic_ensemble      dead_neurons = { };

    template_ensemble_generation(4, 400, 100, { 25, 25, 50, 50 }, expected_ensembles, dead_neurons);
}

TEST(utest_hhn, three_sync_ensembles) {
    basic_ensemble_data expected_ensembles = { { 0, 1 }, { 2, 3 }, { 4, 5 } };
    basic_ensemble      dead_neurons = { };

    template_ensemble_generation(6, 800, 200, { 20, 20, 50, 50, 80, 80 }, expected_ensembles, dead_neurons);
}