# SPDX-FileCopyrightText: 2017-2022 Blender Authors
#
# SPDX-License-Identifier: GPL-2.0-or-later

# ############################################################
# Importing - Same For All Render Layer Tests
# ############################################################

import unittest

from view_layer_common import (
    ViewLayerTesting,
    get_scene_collections,
    setup_extra_arguments,
)


# ############################################################
# Testing
# ############################################################

class UnitTesting(ViewLayerTesting):
    def test_scene_collections_copy_full(self):
        """
        See if scene copying 'FULL_COPY' is working for scene collections
        """
        import os
        ROOT = self.get_root()

        filepath_layers_json_copy = os.path.join(ROOT, 'layers_copy_full_simple.json')
        self.do_scene_copy(
            filepath_layers_json_copy,
            'FULL_COPY',
            (get_scene_collections,))


# ############################################################
# Main - Same For All Render Layer Tests
# ############################################################

if __name__ == '__main__':
    UnitTesting._extra_arguments = setup_extra_arguments(__file__)
    unittest.main()
