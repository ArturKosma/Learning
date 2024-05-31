import numpy as np

class leqmath:

    def dir_to_angle(U, V):
        angle_rad = np.arctan2(V, U)
        angle_deg = np.degrees(angle_rad)
        return angle_deg

    def angle_to_dir(angle):
        angle_rad = np.radians(angle)
        return np.array([np.cos(angle_rad), np.sin(angle_rad)], dtype=float)

    def normalize(vector):
        return vector / np.linalg.norm(vector)
            
            