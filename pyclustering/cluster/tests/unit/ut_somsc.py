"""!

@brief Unit-tests for SOM-SC algorithm.

@authors Andrei Novikov (pyclustering@yandex.ru)
@date 2014-2018
@copyright GNU Public License

@cond GNU_PUBLIC_LICENSE
    PyClustering is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    PyClustering is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
@endcond

"""


import unittest

# Generate images without having a window appear.
import matplotlib
matplotlib.use('Agg')

from pyclustering.cluster.tests.somsc_templates import SyncnetTestTemplates

from pyclustering.samples.definitions import SIMPLE_SAMPLES


class SomscUnitTest(unittest.TestCase):
    def testClusterAllocationSampleSimple1(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE1, 2, [5, 5], False)

    def testClusterOneAllocationSampleSimple1(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE1, 1, [10], False)

    def testClusterAllocationSampleSimple2(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE2, 3, [10, 5, 8], False)

    def testClusterOneAllocationSampleSimple2(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE2, 1, [23], False)

    def testClusterAllocationSampleSimple3(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE3, 4, [10, 10, 10, 30], False)

    def testClusterOneAllocationSampleSimple3(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE3, 1, [60], False)

    def testClusterAllocationSampleSimple4(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE4, 5, [15, 15, 15, 15, 15], False)

    def testClusterOneAllocationSampleSimple4(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE4, 1, [75], False)

    def testClusterAllocationSampleSimple5(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE5, 4, [15, 15, 15, 15], False)

    def testClusterOneAllocationSampleSimple5(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE5, 1, [60], False)

    def testClusterOneDimensionSampleSimple7(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE7, 2, [10, 10], False)

    def testClusterOneDimensionSampleSimple8(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE8, 4, None, False)

    def testWrongNumberOfCentersSimpleSample1(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE1, 3, None, False)

    def testWrongNumberOfCentersSimpleSample2(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE2, 4, None, False)

    def testClusterTheSameData1(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE9, 2, [10, 20], False)

    def testClusterTheSameData2(self):
        SyncnetTestTemplates.templateLengthProcessData(SIMPLE_SAMPLES.SAMPLE_SIMPLE12, 3, [5, 5, 5], False)

    def testClusterAllocationOneDimensionData(self):
        SyncnetTestTemplates.templateClusterAllocationOneDimensionData(False)


if __name__ == "__main__":
    unittest.main()