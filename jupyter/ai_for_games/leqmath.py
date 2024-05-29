import numpy as np

class leqmath:

    def dir_to_angle(U, V):
        angle_rad = np.arctan2(V, U)
        angle_deg = np.degrees(angle_rad)
        return angle_deg
            