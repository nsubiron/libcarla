import carla

import sys
import unittest

from subprocess import check_output


class testClient(unittest.TestCase):
    def test_client_version(self):
        c = carla.Client('localhost', 8080)
        v = c.get_client_version()
        out = check_output(['git', 'describe', '--tags', '--dirty', '--always', ])
        if sys.version_info > (3, 0):
            out = out.decode('utf8')
        self.assertEqual(str(v), str(out.strip()))
