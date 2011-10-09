from numpy import *

"""
http://en.wikipedia.org/wiki/Rotation_matrix
"""

def normalize( v ):
    len = linalg.norm( v )
    v = v / len
    return v

def create_numpy_rotation_matrix( axis, phi ):
    axis_n = normalize( axis )

    u_1 = axis_n[0]
    u_2 = axis_n[1]
    u_3 = axis_n[2]

    cos_phi = cos( phi )
    sin_phi = sin( phi )

    return array([[ cos_phi + ((1 - cos_phi) * u_1 * u_1), (1 - cos_phi) * u_1 * u_2 - u_3 * sin_phi, (1 - cos_phi) * u_1 * u_3 + u_2 * sin_phi ],
[ (1 - cos_phi) * u_1 * u_2 + u_3 * sin_phi, cos_phi + (1 - cos_phi) * u_2 * u_2, (1 - cos_phi) * u_2 * u_3 - u_1 * sin_phi ],
[ (1 - cos_phi) * u_1 * u_3 - u_2 * sin_phi, (1 - cos_phi) * u_2 * u_3 + u_1 * sin_phi, cos_phi + (1 - cos_phi) * u_3 * u_3 ]])


def create_orthogonal_projection_matrix( axis ):
    axis_n = normalize( axis )
    return eye(3) - outer(axis_n, axis_n)


def sampled_inner_product(x,v, n = 100):
    if abs(dot(x,v))>0.99:
        return zeros(n)
    dtheta = 2*pi/n
    P = create_orthogonal_projection_matrix( x )
    Pv = normalize(dot(P,v))
    inner_product = []
    for theta in arange(0,2*pi,dtheta):
        R = create_numpy_rotation_matrix( x, theta )
        RPv = dot(R,Pv)
        inner_product.append(dot(v,RPv))
    return inner_product

def Funk_Radon(x,v,f,n = 100):
    inner_product = sampled_inner_product(x,v,n)
    dtheta = 2*pi/n
    return sum([f(z) for z in inner_product])*dtheta


def assemble_approximation_matrix(tessellation, nodes, f, n = 100):
    """Assemble the kernel matrix from a given set of directions. """
    #use R(fabs(dot(i,j))) in case angles are considered in the range (0,pi/2)
    d1 = tessellation
    d2 = nodes
    return array([ Funk_Radon(i,j,f,n) for i in d1 for j in d2]).reshape(len(d1),len(d2))




