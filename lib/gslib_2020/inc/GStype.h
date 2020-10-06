/******************************************************************************
* File Name : GStype.h                              Ver : 1.00  Date : 98-10-22
*
* Description :
*
*       �Q�[���V�X�e�� ��{�f�[�^�^��`�t�@�C���D
*
* Author : Moriharu Ohzu.
*
******************************************************************************/
#ifndef		_GSTYPE_H_
#define		_GSTYPE_H_

#include	<windows.h>
#include	<GL/gl.h>

#undef min
#undef max

#pragma warning(push)
#pragma warning(disable:4201) // �����\���̂̎g�p
#pragma warning(disable:4458) // �N���X�����o�̉B��


/****** �f�[�^�^�錾 *********************************************************/

/*----- ��{�f�[�^�^ --------------------------------------------------------*/

typedef	unsigned int	GSenum;
typedef	unsigned char	GSboolean;
typedef	unsigned int	GSbitfield;
typedef	signed char		GSbyte;
typedef	short			GSshort;
typedef	int				GSint;
typedef	int				GSsizei;
typedef	unsigned char	GSubyte;
typedef	unsigned short	GSushort;
typedef	unsigned int	GSuint;
typedef	float			GSfloat;
typedef	float			GSclampf;
typedef	double			GSdouble;
typedef	double			GSclampd;
typedef	void			GSvoid;

#define	GS_TRUE			1				/* �^ */
#define	GS_FALSE		0				/* �U */


/* �C�����C���֐��̒�` */
#ifdef __cplusplus
#define	GSINLINE	inline				/* �b�{�{ */
#else
#define	GSINLINE	static _inline		/* �b���� */
#endif

/* �b�{�{�O���Q�� */
#ifdef __cplusplus
struct GSvector2;
struct GSvector3;
struct GSvector4;
struct GSquaternion;
struct GSplane;
struct GSmatrix3;
struct GSmatrix4;
#endif

/*----- �Q�������W�E�Q�����x�N�g���\���� ------------------------------------*/

typedef struct GSvector2
{
	union
	{
		struct
		{
			GSfloat		x;				/* �����W                   */
			GSfloat		y;				/* �����W                   */
		};
		struct
		{
			GSfloat		s;				/* �����W                   */
			GSfloat		t;				/* �����W                   */
		};
		GSfloat			v[2];			/* �������W                 */
	};

#ifdef __cplusplus

	/** �R���X�g���N�^ */
	GSvector2() {}
	GSvector2( GSfloat X, GSfloat Y );
	explicit GSvector2( const GSvector3& v3 );
	explicit GSvector2( const GSvector4& v4 );
	explicit GSvector2( const GSfloat* p );

	/** ���K�� */
	GSvector2& normalize();

	/** ���K�������x�N�g�����擾���� */
	GSvector2 getNormalized() const;

	/** ���������߂� */
	GSfloat length() const;

	/** ���������߂� (�Q��a�̂݁j*/
	GSfloat lengthSq() const;

	/** ���������߂� */
	GSfloat distance( const GSvector2& V ) const;

	/** ���ς����߂� */
	GSfloat dot( const GSvector2& V ) const;

	/** �O�ς����߂� */
	GSfloat CCW( const GSvector2& V ) const;

	/** �������v�f�����߂� */
	GSvector2 minimize( const GSvector2& V ) const;

	/** �傫���v�f�����߂� */
	GSvector2 maximize( const GSvector2& V ) const;

	/** �N�����v���� */
	GSvector2 clamp( const GSvector2& l, const GSvector2& h ) const;

	/** ���`��Ԃ��s�� */
	GSvector2 lerp( const GSvector2& V, GSfloat t ) const;

	/** �Q����Ԃ��s�� */
	GSvector2 quadratic( const GSvector2& V2, const GSvector2& V3, GSfloat t ) const;

	/** ���˃x�N�g�������߂� */
	GSvector2 reflect( const GSvector2& N ) const;

	/** ���܃x�N�g�������߂� */
	GSvector2 refract( const GSvector2& N, GSfloat Eta ) const;

	/** �Q�x�N�g���̂Ȃ��p�x�����߂� */
	GSfloat degree( const GSvector2& V ) const;

	/** ���ʊp�����߂� */
	GSfloat getDirection() const;

	/** ���ʊp����x�N�g�������߂� */
	GSvector2& fromDirection(GSfloat direction);

	/** ���ʊp����x�N�g�������߂� */
	static GSvector2 createFromDirection(GSfloat direction);

	/** ��̃x�N�g���̂Ȃ��p�x�����߂� */
	GSfloat innerDegree(const GSvector2& other) const;

	/** �L���X�g���Z�q�I�[�o�[���[�h */
	operator GSfloat* () const;
	operator const GSfloat* () const;

	/** �P�����Z�q�I�[�o�[���[�h */
	GSvector2  operator + () const;
	GSvector2  operator - () const;

	/** ������Z�q�I�[�o�[���[�h */
	GSvector2&  operator += ( const GSvector2& v );
	GSvector2&  operator -= ( const GSvector2& v );
	GSvector2&  operator *= ( const GSvector2& v );
	GSvector2&  operator /= ( const GSvector2& v );
	GSvector2&  operator *= ( GSfloat s );
	GSvector2&  operator /= ( GSfloat s );

	/** �Q�����Z�q�I�[�o�[���[�h */
	friend const GSvector2 operator + ( const GSvector2& v1, const GSvector2& v2 );
	friend const GSvector2 operator - ( const GSvector2& v1, const GSvector2& v2 );
	friend const GSvector2 operator * ( const GSvector2& v, const GSfloat s );
	friend const GSvector2 operator * ( const GSfloat s, const GSvector2& v );
	friend const GSvector2 operator * ( const GSmatrix3& m, const GSvector2& v );
	friend const GSvector2 operator * ( const GSvector2& v, const GSmatrix3& m );
	friend const GSvector2 operator / ( const GSvector2& v, const GSfloat s );
	friend bool operator == ( const GSvector2& v1, const GSvector2& v2 );
	friend bool operator != ( const GSvector2& v1, const GSvector2& v2 );
	friend bool operator < ( const GSvector2& v1, const GSvector2& v2 );
	friend bool operator > ( const GSvector2& v1, const GSvector2& v2 );
	friend bool operator <= ( const GSvector2& v1, const GSvector2& v2 );
	friend bool operator >= ( const GSvector2& v1, const GSvector2& v2 );

	// --- Unity�݊��C���^�[�t�F�[�X�̒ǉ� 2020/04/13 

	// GSvector2(0, 0) �Ɠ����Ӗ�
	static GSvector2 zero();
	// GSvector2(1, 1) �Ɠ����Ӗ�
	static GSvector2 one();
	// GSvector2(0, 1) �Ɠ����Ӗ�
	static GSvector2 up();
	// GSvector2(0, -1) �Ɠ����Ӗ�
	static GSvector2 down();
	// GSvector2(-1, 0) �Ɠ����Ӗ�
	static GSvector2 left();
	// GSvector2(1, 0) �Ɠ����Ӗ�
	static GSvector2 right();

	// �x�N�g���̒����i�ǂݎ���p�j
	float magnitude() const;
	// magnitude �� 1 �Ƃ����x�N�g���i�ǂݎ���p�j
	GSvector2 normalized() const;
	//�@�x�N�g���� 2 ��̒�����Ԃ��܂��i�ǂݎ���p�j
	float sqrMagnitude() const;

	// [0] ��[1] ���g�p���� x �� y �����ɃA�N�Z�X���܂�
	float operator[](int index) const;
	float& operator[](int index);

	// ������ GSvector2 �� x �� y �̐�����ݒ肵�܂�
	void set(float x, float y);

	// 2�̃x�N�g���̂Ȃ��p��Ԃ��܂�
	static float angle(const GSvector2& from, const GSvector2& to);
	// �傫���� maxLength �܂łɐ������� vector �̃R�s�[��Ԃ��܂�
	static GSvector2 clampMagnitude(const GSvector2& vector, float max_length);
	// a �� b �̊Ԃ̋�����Ԃ��܂�
	static float distance(const GSvector2& a, const GSvector2& b);
	// 2 �̃x�N�g���̓���
	static float dot(const GSvector2& lhs, const GSvector2& rhs);

	// 2�x�N�g��a��b�̐��`��Ԃ��܂��B
	static GSvector2 lerp(const GSvector2& a, const GSvector2& b, float t);
	// 2�x�N�g��a �� b �̐��`��Ԃ��܂��B
	static GSvector2 lerpUnclamped(const GSvector2& a, const GSvector2& b, float t);
	// 2�̃x�N�g���Ŋe�����̈�ԑ傫�Ȓl���g�p���ăx�N�g�����쐬���܂�
	static GSvector2 max(const GSvector2& lhs, const GSvector2& rhs);
	// 2 �̃x�N�g���Ŋe�����̈�ԏ����Ȓl���g�p���ăx�N�g�����쐬���܂�
	static GSvector2 min(const GSvector2& lhs, const GSvector2& rhs);
	// ���݂̈ʒu current ���� target �Ɍ����Ĉړ����܂�
	static GSvector2 moveTowards(const GSvector2& current, const GSvector2& target,
		float max_distance_delta);
	// ����2D�x�N�g���ɐ�����2D�x�N�g����Ԃ��܂��B
	// ����Y�����オ��2D���W�n�ł́A���ʂ͏�ɔ����v����90�x��]���܂��B
	static GSvector2 perpendicular(const GSvector2& inDirection);
	// �@������ɂ��ăx�N�g���̔��˂����x�N�g�����擾���܂�
	static GSvector2 reflect(const GSvector2& inDirection, const GSvector2& inNormal);
	// 2�̃x�N�g���̊e��������Z���܂�
	static GSvector2 scale(const GSvector2& a, const GSvector2& b);
	// Returns the signed angle in degrees between from and to.
	static float signedAngle(const GSvector2& from, const GSvector2& to);
	// �ړI�n�Ɍ������Ď��Ԃ̌o�߂ƂƂ��ɏ��X�Ƀx�N�g����ω������܂�
	GSvector2 smoothDamp(const GSvector2& current, const GSvector2& target,
		GSvector2& currentVelocity, float smooth_time, float max_speed, float delta_time);
#endif

} GSvector2;

/*----- �R�������W�E�R�����x�N�g���\���� ------------------------------------*/

typedef struct GSvector3
{
	union
	{
		struct
		{
			GSfloat		x;				/* �����W                   */
			GSfloat		y;				/* �����W                   */
			GSfloat		z;				/* �����W                   */
		};
		struct
		{
			GSfloat		s;				/* �����W                   */
			GSfloat		t;				/* �����W                   */
			GSfloat		p;				/* �����W                   */
		};
		struct
		{
			GSfloat		r;				/* �Ԑ���                   */
			GSfloat		g;				/* �ΐ���                   */
			GSfloat		b;				/* ����                   */
		};
		struct
		{
			GSvector2	xy;				/* �������W                 */
		};
		GSfloat			v[3];			/* ���������W               */
	};

#ifdef __cplusplus

	/** �R���X�g���N�^ */
	GSvector3() {}
	GSvector3( GSfloat X, GSfloat Y, GSfloat Z );
	GSvector3( const GSvector2& v2, GSfloat Z = 0.0f );
	explicit GSvector3( const GSvector4& v4 );
	explicit GSvector3( const GSfloat* p );

	/** ���K�� */
	GSvector3& normalize();

	/** ���K�������x�N�g�����擾���� */
	GSvector3 getNormalized() const;

	/** ���������߂� */
	GSfloat length() const;

	/** ���������߂� (�Q��a�̂݁j*/
	GSfloat lengthSq() const;

	/** ���������߂� */
	GSfloat distance( const GSvector3& V ) const;

	/** ���ς����߂� */
	GSfloat dot( const GSvector3& V ) const;

	/** �O�ς����߂� */
	GSvector3 cross( const GSvector3& V ) const;

	/** �������v�f�����߂� */
	GSvector3 minimize( const GSvector3& V ) const;

	/** �傫���v�f�����߂� */
	GSvector3 maximize( const GSvector3& V ) const;

	/** �N�����v���� */
	GSvector3 clamp( const GSvector3& l, const GSvector3& h ) const;

	/** ���`��Ԃ��s�� */
	GSvector3 lerp( const GSvector3& V, GSfloat t ) const;

	/** �Q����Ԃ��s�� */
	GSvector3 quadratic( const GSvector3& V2, const GSvector3& V3, GSfloat t ) const;

	/** ���˃x�N�g�������߂� */
	GSvector3 reflect( const GSvector3& N ) const;

	/** ���܃x�N�g�������߂� */
	GSvector3 refract( const GSvector3& N, GSfloat Eta ) const;

	/** �Q�x�N�g���̂Ȃ��p�x�����߂� */
	GSfloat degree( const GSvector3& V ) const;

	/** ���[�����߂� */
	GSfloat getYaw() const;

	/** �s�b�`�����߂� */
	GSfloat getPitch() const;

	/** �s�b�`�E���[����x�N�g�������߂� */
	GSvector3& fromPitchYaw( GSfloat picth, GSfloat yow );

	/** �s�b�`�E���[����x�N�g�������߂� */
	static GSvector3 createFromPitchYaw(GSfloat picth, GSfloat yow);

	/** �@���x�N�g�������߂� */
	GSvector3 calculateNormal( const GSvector3& V1, const GSvector3& V2 ) const;

	/** ��̃x�N�g���̂Ȃ��p�x�����߂� */
	GSfloat innerDegree( const GSvector3& other ) const;

	/** �L���X�g���Z�q�I�[�o�[���[�h */
	operator GSfloat* () const;
	operator const GSfloat* () const;

	/** �P�����Z�q�I�[�o�[���[�h */
	GSvector3  operator + () const;
	GSvector3  operator - () const;

	/** ������Z�q�I�[�o�[���[�h */
	GSvector3&  operator += ( const GSvector3& v );
	GSvector3&  operator -= ( const GSvector3& v );
	GSvector3&  operator *= ( const GSvector3& v );
	GSvector3&  operator /= ( const GSvector3& v );
	GSvector3&  operator *= ( GSfloat s );
	GSvector3&  operator /= ( GSfloat s );

	/** �Q�����Z�q�I�[�o�[���[�h */
	friend const GSvector3 operator + ( const GSvector3& v1, const GSvector3& v2 );
	friend const GSvector3 operator - ( const GSvector3& v1, const GSvector3& v2 );
	friend const GSvector3 operator * ( const GSvector3& v, const GSfloat s );
	friend const GSvector3 operator * ( const GSfloat s, const GSvector3& v );
	friend const GSvector3 operator * ( const GSmatrix4& m, const GSvector3& v );
	friend const GSvector3 operator * ( const GSvector3& v, const GSmatrix4& m );
	friend const GSvector3 operator * ( const GSmatrix3& m, const GSvector3& v );
	friend const GSvector3 operator * ( const GSvector3& v, const GSmatrix3& m );
	friend const GSvector3 operator / ( const GSvector3& v, const GSfloat s );
	friend bool operator == ( const GSvector3& v1, const GSvector3& v2 );
	friend bool operator != ( const GSvector3& v1, const GSvector3& v2 );
	friend bool operator < ( const GSvector3& v1, const GSvector3& v2 );
	friend bool operator > ( const GSvector3& v1, const GSvector3& v2 );
	friend bool operator <= ( const GSvector3& v1, const GSvector3& v2 );
	friend bool operator >= ( const GSvector3& v1, const GSvector3& v2 );

	// --- Unity�݊��C���^�[�t�F�[�X�̒ǉ� 2020/04/13 

	// �x�N�g���̒���
	float magnitude() const;
	// magnitude �� 1 �Ƃ����x�N�g��
	GSvector3 normalized() const;
	// �x�N�g����2��̒���
	float sqr_magnitude() const;
	// [0] ��[1] ���g�p���� x �� y �����ɃA�N�Z�X���܂�
	float operator[](int index) const;
	float& operator[](int index);

	// 2�̃x�N�g���̊e��������Z���܂�
	void scale(const GSvector3& v);
	// ������ Vector3 �� x�Ay�Az �̐�����ݒ肵�܂�
	void set(float new_x, float new_y, float new_z);

	// 2�_�ԁifrom �� to�j�̊p�x��Ԃ��܂�
	static float angle(const GSvector3& from, const GSvector3& to);
	// �傫����max_length�܂łɐ������� vector �̃R�s�[��Ԃ��܂�
	static GSvector3 clampMagnitude(const GSvector3& vector, float max_length);
	// 2�̃x�N�g���̊O��
	static GSvector3 cross(const GSvector3& lhs, const GSvector3& rhs);
	// a �� b �̊Ԃ̋�����Ԃ��܂�
	static float distance(const GSvector3& a, const GSvector3& b);
	// 2�̃x�N�g���̓���
	static float dot(const GSvector3& lhs, const GSvector3& rhs);

	// ������ɂ��� 2 �̃x�N�g���Ԃ��Ԃ��܂�
	static GSvector3 lerp(const GSvector3& a, const GSvector3& b, float t);
	// ������ɂ��� 2 �̃x�N�g���Ԃ��Ԃ��܂�
	static GSvector3 lerpUnclamped(const GSvector3& a, const GSvector3& b, float t);
	// 2�̃x�N�g���Ŋe�����̈�ԑ傫�Ȓl���g�p���ăx�N�g�����쐬���܂�
	static GSvector3 max(const GSvector3& lhs, const GSvector3& rhs);
	// 2�̃x�N�g���Ŋe�����̈�ԏ����Ȓl���g�p���ăx�N�g�����쐬���܂�
	static GSvector3 min(const GSvector3& lhs, const GSvector3& rhs);

	// ���݂̈ʒu current ���� target �Ɍ����Ĉړ����܂�
	static GSvector3 moveTowards(const GSvector3& current, const GSvector3& target, float max_distance_delta);
	// magnitude �� 1 �Ƃ����x�N�g�����쐬���܂�
	static GSvector3 normalize(const GSvector3& value);
	// �x�N�g�������K�����ꑼ�̃x�N�g���ƒ�������悤�ɂ��܂�
	static void orthoNormalize(GSvector3& normal, GSvector3& tangent, GSvector3& binormal);

	// �x�N�g����ʂ̃x�N�g���ɓ��e���܂�
	static GSvector3 project(const GSvector3& vector, const GSvector3& onNormal);
	// ���ʂɐ����Ȗ@���x�N�g���ɂ���Ē�`����镽�ʏ�Ƀx�N�g�����ˉe���܂�
	static GSvector3 projectOnPlane(const GSvector3& vector, const GSvector3& planeNormal);
	// �@���Œ�`���ꂽ���ʂŃx�N�g���𔽎˂��܂�
	static GSvector3 reflect(const GSvector3& inDirection, const GSvector3& inNormal);
	// ���݂̈ʒu current ���� target �Ɍ����ăx�N�g������]���܂��B
	static GSvector3 rotate_towards(const GSvector3& current, const GSvector3& target, float max_radian_delta, float max_magnitude_delta);
	// 2�̃x�N�g���̊e��������Z���܂�
	static GSvector3 scale(const GSvector3& a, const GSvector3& b);
	// 2�_�ԁifrom �� to�j�̕����t���p�x��Ԃ��܂�
	static float signed_angle(const GSvector3& from, const GSvector3& to, const GSvector3& axis = up());
	// ����� 2 �̃x�N�g���Ԃ��Ԃ��܂�
	static GSvector3 slerp(const GSvector3& a, const GSvector3& b, float t);
	// ����� 2 �̃x�N�g���Ԃ��Ԃ��܂�
	static GSvector3 slerpUnclamped(const GSvector3& a, const GSvector3& b, float t);

	// �ړI�n�Ɍ������Ď��Ԃ̌o�߂ƂƂ��ɏ��X�Ƀx�N�g����ω������܂�
	static GSvector3 smoothDamp(const GSvector3& current, const GSvector3& target, GSvector3& current_velocity, float smooth_time, float max_speed, float delta_time);

	// Vector3(0, 0, 0) �Ɠ����Ӗ�
	static GSvector3 zero();
	// Vector3(1, 1, 1) �Ɠ����Ӗ�
	static GSvector3 one();
	// Vector3(0, 1, 0) �Ɠ����Ӗ�
	static GSvector3 up();
	// Vector3(0, -1, 0) �Ɠ����Ӗ�
	static GSvector3 down();
	// Vector3(1, 0, 0) �Ɠ����Ӗ�
	static GSvector3 left();
	// Vector3(-1, 0, 0) �Ɠ����Ӗ�
	static GSvector3 right();
	// Vector3(0, 0, 1) �Ɠ����Ӗ�
	static GSvector3 forward();
	// Vector3(0, 0, -1) �Ɠ����Ӗ�
	static GSvector3 back();
	// ���̖�����
	static GSvector3 positiveInfinity();
	// ���̖�����
	static GSvector3 negativeInfinity();
#endif

} GSvector3;

/*----- �S�������W�E�S�����x�N�g���\���� ------------------------------------*/

typedef struct GSvector4
{
	union
	{
		struct
		{
			GSfloat		x;				/* �����W                   */
			GSfloat		y;				/* �����W                   */
			GSfloat		z;				/* �����W                   */
			GSfloat		w;				/* �����W                   */
		};
		struct
		{
			GSfloat		s;				/* �����W                   */
			GSfloat		t;				/* �����W                   */
			GSfloat		p;				/* �����W                   */
			GSfloat		q;				/* �����W                   */
		};
		struct
		{
			GSfloat		r;				/* �Ԑ���                   */
			GSfloat		g;				/* �ΐ���                   */
			GSfloat		b;				/* ����                   */
			GSfloat		a;				/* �A���t�@����             */
		};
		struct
		{
			GSvector2	xy;				/* �������W                 */
		};
		struct
		{
			GSvector3	xyz;			/* ���������W               */
		};
		struct
		{
			GSvector3	rgb;			/* �q�f�a                   */
		};
		GSfloat			v[4];			/* �����������W             */
	};

#ifdef __cplusplus

	/** �R���X�g���N�^ */
	GSvector4() {}
	GSvector4( GSfloat X, GSfloat Y, GSfloat Z, GSfloat W );
	GSvector4( const GSvector2& v2, GSfloat Z = 0.0f, GSfloat W = 0.0f );
	GSvector4( const GSvector3& v3, GSfloat W = 0.0f );
	explicit GSvector4( const GSfloat* p );

	/** ���K�������x�N�g�����擾���� */
	GSvector4& normalize();

	/** ���K�������x�N�g�����擾���� */
	GSvector4 getNormalized() const;

	/** ���������߂� */
	GSfloat length() const;

	/** ���������߂� (�Q��a�̂݁j*/
	GSfloat lengthSq() const;

	/** ���������߂� */
	GSfloat distance( const GSvector4& V ) const;

	/** ���ς����߂� */
	GSfloat dot( const GSvector4& V ) const;

	/** �������v�f�����߂� */
	GSvector4 minimize( const GSvector4& V ) const;

	/** �傫���v�f�����߂� */
	GSvector4 maximize( const GSvector4& V ) const;

	/** �N�����v���� */
	GSvector4 clamp( const GSvector4& l, const GSvector4& h ) const;

	/** ���`��Ԃ��s�� */
	GSvector4 lerp( const GSvector4& V, GSfloat t ) const;

	/** �Q����Ԃ��s�� */
	GSvector4 quadratic( const GSvector4& V2, const GSvector4& V3, GSfloat t ) const;

	/** �L���X�g���Z�q�I�[�o�[���[�h */
	operator GSfloat* () const;
	operator const GSfloat* () const;

	/** �P�����Z�q�I�[�o�[���[�h */
	GSvector4 operator + () const;
	GSvector4 operator - () const;

	/** ������Z�q�I�[�o�[���[�h */
	GSvector4& operator += ( const GSvector4& v );
	GSvector4& operator -= ( const GSvector4& v );
	GSvector4& operator *= ( const GSvector4& v );
	GSvector4& operator /= ( const GSvector4& v );
	GSvector4& operator *= ( GSfloat s );
	GSvector4& operator /= ( GSfloat s );

	/** �Q�����Z�q�I�[�o�[���[�h */
	friend const GSvector4 operator + ( const GSvector4& v1, const GSvector4& v2 );
	friend const GSvector4 operator - ( const GSvector4& v1, const GSvector4& v2 );
	friend const GSvector4 operator * ( const GSvector4& v, const GSfloat s );
	friend const GSvector4 operator * ( const GSfloat s, const GSvector4& v );
	friend const GSvector4 operator * ( const GSmatrix4& m, const GSvector4& v );
	friend const GSvector4 operator * ( const GSvector4& v, const GSmatrix4& m );
	friend const GSvector4 operator / ( const GSvector4& v, const GSfloat s );
	friend bool operator == ( const GSvector4& v1, const GSvector4& v2 );
	friend bool operator != ( const GSvector4& v1, const GSvector4& v2 );
	friend bool operator < ( const GSvector4& v1, const GSvector4& v2 );
	friend bool operator > ( const GSvector4& v1, const GSvector4& v2 );
	friend bool operator <= ( const GSvector4& v1, const GSvector4& v2 );
	friend bool operator >= ( const GSvector4& v1, const GSvector4& v2 );

    // ������ɂ��� 2 �̃x�N�g���Ԃ��Ԃ��܂�
    static GSvector4 lerp(const GSvector4& a, const GSvector4& b, float t);
    // ������ɂ��� 2 �̃x�N�g���Ԃ��Ԃ��܂�
    static GSvector4 lerpUnclamped(const GSvector4& a, const GSvector4& b, float t);

#endif

} GSvector4;

/*----- �J���[�\���� --------------------------------------------------------*/

typedef	struct GSvector4	GScolor;		/* �J���[�^			*/

/*----- ���ʍ\���� ----------------------------------------------------------*/

typedef struct GSplane
{
	union
	{
		struct
		{
			GSfloat			a;			/* �ʖ@��������             */
			GSfloat			b;			/* �ʖ@��������             */
			GSfloat			c;			/* �ʖ@��������             */
			GSfloat			d;			/* ���_����̋���           */
		};
		struct
		{
			GSvector3		normal;		/* �ʖ@���x�N�g��           */
		};
	};

#ifdef __cplusplus

	GSplane() {}
	GSplane( GSfloat A, GSfloat B, GSfloat C, GSfloat D );
	GSplane( const GSvector3& N, GSfloat D );
	GSplane( const GSvector3& P, const GSvector3& N );
	GSplane( const GSvector3& V1, const GSvector3& V2, const GSvector3& V3 );

	/** ���K������ */
	GSplane& normalize();

	/** ���K����̕��ʂ𓾂� */
	GSplane getNormalized();

	/** ���ʏ�̈ʒu���擾 */
	GSvector3 getPoint() const;

	/** ���ʂƃx�N�g���̓��ς����߂� */
	GSfloat dot( const GSvector3& v ) const;

	/** ���ʂƓ_�̋��������߂� */
	GSfloat distance( const GSvector3& v ) const;

	/** ���ʂƐ������������邩���ׂ� */
	bool isIntersectLine( const GSvector3& Line0,  const GSvector3& Line1 ) const;

	/** ���ʂƐ����̌�_���擾���� */
	GSvector3 getIntersectLine( const GSvector3& Line0,  const GSvector3& Line1 ) const;

	/** ���ʂƐ����̌�_���擾���� */
	bool isIntersectRay( const GSvector3& Line0,  const GSvector3& Line1 ) const;

	/** ���ʂƐ����̌�_���擾���� */
	GSvector3 getIntersectRay( const GSvector3& Line0,  const GSvector3& Line1 ) const;

	/** ���ʂƍ��W�̋��������߂� */
	GSfloat getDistance( const GSvector3& p ) const;

	/** ���ʂƍ��W�̈ʒu�֌W��Ԃ� */
	GSenum getClassifyPoint( const GSvector3& p ) const;

	/** ���ʂƋ��̂̈ʒu�֌W��Ԃ� */
	GSenum getClassifyShpere( const GSvector3& center, GSfloat Radius ) const;

	/** ���`��Ԃ��s�� */
	GSplane lerp( const GSplane& V, GSfloat t ) const;

	/** �Q����Ԃ��s�� */
	GSplane quadratic( const GSplane& V2, const GSplane& V3, GSfloat t ) const;

	/** �P�����Z�q�I�[�o�[���[�h */
	GSplane  operator + () const;
	GSplane  operator - () const;

	/** �Q�����Z�q�I�[�o�[���[�h */
	friend const GSplane operator * ( const GSmatrix3& m, const GSplane& p );
	friend const GSplane operator * ( const GSplane& p, const GSmatrix3& m );
	friend const GSplane operator * ( const GSmatrix4& m, const GSplane& p );
	friend const GSplane operator * ( const GSplane& p, const GSmatrix4& m );

#endif

} GSplane;

/*----- �N�H�[�^�j�I���\���� -------------------------------------------------*/

typedef struct GSquaternion
{
	GSfloat			x;					/* ����						*/
	GSfloat			y;					/* ����						*/
	GSfloat			z;					/* ����						*/
	GSfloat			w;					/* �p�x						*/

#ifdef __cplusplus

	GSquaternion() {}
	GSquaternion( GSfloat X, GSfloat Y, GSfloat Z, GSfloat W );
	GSquaternion( GSfloat angle, const GSvector3& axis );
	explicit GSquaternion( const GSmatrix3& m );
	explicit GSquaternion( const GSmatrix4& m );
	GSquaternion( const GSvector3& xAxis, const GSvector3& yAxis, const GSvector3& zAxis );


	/** ���� */
	GSfloat dot( const GSquaternion& q ) const;

	/** ���������߂� */
	GSfloat length() const;

	/** ���K�����s�� */
	GSquaternion& normalize();

	/** �t�N�H�[�^�j�I�������� */
	GSquaternion& inverse();

	/** ���K����̃N�H�[�^�j�I�����擾 */
	GSquaternion getNormalized() const;

	/** �t�N�H�[�^�j�I�������擾 */
	GSquaternion getInverse () const;

	/** ��]�p�x����N�H�[�^�j�I����ݒ� */
	GSquaternion& setRotate( GSfloat angle, GSfloat x, GSfloat y, GSfloat z );
	GSquaternion& setRotate( GSfloat angle, const GSvector3& axis );

	/** ��]�p�x����N�H�[�^�j�I����ݒ� */
	GSquaternion& setRotateYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll );

	/** ��]�s���ݒ� */
	GSquaternion& setMatrix( const GSmatrix3& m );
	GSquaternion& setMatrix( const GSmatrix4& m );

	/** ���Ɗp�x���擾 */
	GSvector3 getAxis() const;
	GSfloat   getAngle() const;

	/** ���[�C�s�b�`�C���[���̎擾 */
	GSfloat getYaw() const;
	GSfloat getPitch() const;
	GSfloat getRoll() const;

	/** ���ʐ��`��� */
	GSquaternion slerp( const GSquaternion& q, GSfloat t ) const;

	/** ���`��� */
	GSquaternion lerp( const GSquaternion& q, GSfloat t ) const;

	/** �P�����Z�q�I�[�o�[���[�h */
	GSquaternion operator + () const;
	GSquaternion operator - () const;

	/** ������Z�q�I�[�o�[���[�h */
	GSquaternion& operator += ( const GSquaternion& v );
	GSquaternion& operator -= ( const GSquaternion& v );
	GSquaternion& operator *= ( const GSquaternion& v );
	GSquaternion& operator *= ( GSfloat s );
	GSquaternion& operator /= ( GSfloat s );

	/** �Q�����Z�q�I�[�o�[���[�h */
	friend const GSquaternion operator + ( const GSquaternion& q1, const GSquaternion& q2 );
	friend const GSquaternion operator - ( const GSquaternion& q1, const GSquaternion& q2 );
	friend const GSquaternion operator * ( const GSquaternion& q1, const GSquaternion& q2 );
	friend const GSquaternion operator * ( const GSquaternion& q, const GSfloat s );
	friend const GSquaternion operator * ( const GSfloat s, const GSquaternion& q );
	friend bool operator == ( const GSquaternion& q1, const GSquaternion& q2 );
	friend bool operator != ( const GSquaternion& q1, const GSquaternion& q2 );

	// --- Unity�݊��C���^�[�t�F�[�X�̒ǉ� 2020/04/13

	// �P�ʃN�H�[�^�j�I��
	static GSquaternion identity();
	// ��]���I�C���[�p�̒l�ŕԂ��܂�
	GSvector3 eulerAngles() const;
	void eulerAngles(const GSvector3 & value);
	// ���K��
	GSquaternion normalized() const;
	// [0] ��[1] ���g�p���� x �� y �����ɃA�N�Z�X���܂�
	float operator[](int index) const;
	float& operator[](int index);

	// ������ Quaternion �� x�Ay�Az�Aw �̐�����ݒ肵�܂�
	void set(float newX, float newY, float newZ, float newW);
	// fromDirection ���� toDirection �ւ̉�]���쐬���܂�
	void setFromToRotation(const GSvector3& fromDirection, const GSvector3& toDirection);
	// �w�肳�ꂽ forward �� upwards �����ɉ�]���܂�
	void setLookRotation(const GSvector3& view, const GSvector3& up = GSvector3::up());
	// ��]�����W�ɑ΂���p�x�̒l (AngleAxis) �ɕϊ����܂��B
	void toAngleAxis(float& angle, GSvector3& axis);

	// 2 �̉�] a �� b �Ԃ̊p�x��Ԃ��܂��B
	static float angle(const GSquaternion& a, const GSquaternion& b);
	// axis �̎���� angle �x��]�����]���쐬���܂��B
	static GSquaternion angleAxis(float angle, const GSvector3 & axis);
	// 2 �̉�]�̓��ς�Ԃ��܂��B
	static float dot(const GSquaternion& a, const GSquaternion& b);
	// z���𒆐S��z�x�Ax���𒆐S��x�x�Ay���𒆐S��y�x��]�����]��Ԃ��܂��B
	static GSquaternion euler(float x, float y, float z);
	static GSquaternion euler(const GSvector3& euler);
	// fromDirection ���� toDirection �ւ̉�]���쐬���܂��B
	static GSquaternion fromToRotation(const GSvector3& fromDirection, const GSvector3& toDirection);
	// �t�N�H�[�^�j�I����Ԃ��܂�
	static GSquaternion inverse(const GSquaternion& rotation);
	// �w�肳�ꂽ forward �� upwards �����ɉ�]���܂�
	static GSquaternion lookRotation(const GSvector3& view, const GSvector3& up = GSvector3::up());
	// ���K�������N�H�[�^�j�I����Ԃ��܂�
	static GSquaternion normalize(const GSquaternion& q);
	// from ���� to �ւ̉�]�𓾂܂�
	static GSquaternion rotateTowards(const GSquaternion& from, const GSquaternion& to, float maxDegreesDelta);
	//�@a �� b �̊Ԃ� t �Ő��`�ɕ�Ԃ��܂��B�p�����[�^�[ t �́A[0, 1] �͈̔͂ł��B
	static GSquaternion lerp(const GSquaternion& a, const GSquaternion& b, float t);
	// a �� b �̊Ԃ� t �Ő��`�ɕ�Ԃ��܂��B�p�����[�^�[ t �́A�����Ă��܂���B
	static GSquaternion lerpUnclamped(const GSquaternion& a, const GSquaternion& b, float t);
	//�@a �� b �̊Ԃ� t �ŋ���ɕ�Ԃ��܂��B�p�����[�^�[ t �́A[0, 1] �͈̔͂ł��B
	static GSquaternion slerp(const GSquaternion& a, const GSquaternion& b, float t);
	// a �� b �̊Ԃ� t �ŋ���ɕ�Ԃ��܂��B�p�����[�^�[ t �́A�����Ă��܂���B
	static GSquaternion slerpUnclamped(const GSquaternion& a, const GSquaternion& b, float t);
	// ���W�ϊ�
	friend GSvector3 operator * (const GSquaternion& rotation, const GSvector3& point);

#endif

} GSquaternion;

/*----- �R�s�R����W�ϊ��}�g���N�X�\���� ------------------------------------*/

typedef struct GSmatrix3
{
	union
	{
		/* �R�s�R��̍s��           */
		GSfloat		m[3][3];
		struct
		{
			GSfloat	_11, _12, _13;
			GSfloat	_21, _22, _23;
			GSfloat	_31, _32, _33;
		};
		GSfloat		v[9];
	};

#ifdef __cplusplus

	/** �R���X�g���N�^ */
	GSmatrix3() {}
	GSmatrix3(
		GSfloat	m11, GSfloat m12, GSfloat m13,
		GSfloat	m21, GSfloat m22, GSfloat m23,
		GSfloat	m31, GSfloat m32, GSfloat m33 );
	GSmatrix3( const GSvector3& axisX,
						const GSvector3& axisY,
						const GSvector3& axisZ );
	explicit GSmatrix3( const GSfloat* V );
	explicit GSmatrix3( const GSmatrix4& M );
	explicit GSmatrix3( const GSquaternion& q );

	/** �P�ʍs�񉻂��� */
	GSmatrix3& identity();

	/** �s��̐��K���@*/
	GSmatrix3& normalize();
	/** �s��̐��K���@*/
	GSmatrix3 getNormalized() const;

	/** �g��k���s�����Z */
	GSmatrix3& scale( GSfloat x, GSfloat y, GSfloat z );
	GSmatrix3& scale( const GSvector3& scaleFactor );
	GSmatrix3& scale( GSfloat scaleFactor );

	/** ��]����Z */
	GSmatrix3& rotate( GSfloat angle, GSfloat axisX, GSfloat axisY, GSfloat axisZ );
	GSmatrix3& rotate( GSfloat angle, const GSvector3& axis );
	GSmatrix3& rotate( const GSquaternion& q );
	GSmatrix3& rotateX( GSfloat angle );
	GSmatrix3& rotateY( GSfloat angle );
	GSmatrix3& rotateZ( GSfloat angle );
	GSmatrix3& rotateYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll );

	/** ���s�ړ�����Z */
	GSmatrix3& translate( GSfloat x, GSfloat y );
	GSmatrix3& translate( const GSvector2& translation );

	/** �t�s��ɂ��� */
	GSmatrix3& inverse();

	/** �]�u�s��ɂ��� */
	GSmatrix3& transpose();

	/** �]�u�t�s��ɂ��� */
	GSmatrix3& inverseTranspose();

	/** �t�s����擾 */
	GSmatrix3 getInverse() const;

	/** �]�u�s����擾 */
	GSmatrix3 getTranspose() const;

	/** �]�u�t�s����擾 */
	GSmatrix3 getInverseTranspose() const;

	/** �X�P�[���l�̐ݒ� */
	GSmatrix3& setScale( GSfloat x, GSfloat y, GSfloat z );
	GSmatrix3& setScale( const GSvector3& scaleFactor );
	GSmatrix3& setScale( GSfloat scaleFactor );

	/** ��]�p�x�̐ݒ� */
	GSmatrix3& setRotation( GSfloat angle, GSfloat axisX, GSfloat axisY, GSfloat axisZ );
	GSmatrix3& setRotation( GSfloat angle, const GSvector3& axis );
	GSmatrix3& setRotation( const GSquaternion& q );
	GSmatrix3& setRotationX( GSfloat angle );
	GSmatrix3& setRotationY( GSfloat angle );
	GSmatrix3& setRotationZ( GSfloat angle );
	GSmatrix3& setRotationYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll );

	/** ���s�ړ��ʂ̐ݒ� */
	GSmatrix3& setTranslation( GSfloat x, GSfloat y );
	GSmatrix3& setTranslation( const GSvector2& translation );

	/** ���W�ϊ� */
	GSvector2 transform( const GSvector2& v ) const;
	GSvector3 transform( const GSvector3& v ) const;
	GSplane   transform( const GSplane& p ) const;
	GSvector2 transformNormal( const GSvector2& v ) const;

	/** ���C�ʒu�̐ݒ� */
	GSmatrix3& setAxisX( const GSvector3& axisX );
	GSmatrix3& setAxisY( const GSvector3& axisY );
	GSmatrix3& setAxisZ( const GSvector3& axisZ );

	/** ���[���E�s�b�`�E���[�̎擾 */
	GSfloat getRoll() const;
	GSfloat getPitch() const;
	GSfloat getYaw() const;
	void setPosition( const GSvector2& position );

	/** �N�H�[�^�j�I�����擾 */
	GSquaternion getQuaternion() const;

	/** ���C�ʒu�̎擾 */
	GSvector3 getAxisX() const;
	GSvector3 getAxisY() const;
	GSvector3 getAxisZ() const;
	GSvector2 getPosition() const;
	GSvector3 getScale() const;

	/** �s��̕�� */
	GSmatrix3 lerp(const GSmatrix3& other, float t) const;

	/** �L���X�g���Z�q�I�[�o�[���[�h */
	operator GSfloat* () const;
	operator const GSfloat* () const;

	/** �P�����Z�q�I�[�o�[���[�h */
	GSmatrix3 operator + () const;
	GSmatrix3 operator - () const;

	/** ������Z�q�I�[�o�[���[�h */
	GSmatrix3& operator += ( const GSmatrix3& v );
	GSmatrix3& operator -= ( const GSmatrix3& v );
	GSmatrix3& operator *= ( const GSmatrix3& v );
	GSmatrix3& operator *= ( GSfloat s );
	GSmatrix3& operator /= ( GSfloat s );

	/** �Q�����Z�q�I�[�o�[���[�h */
	friend const GSmatrix3 operator + ( const GSmatrix3& m1, const GSmatrix3& m2 );
	friend const GSmatrix3 operator - ( const GSmatrix3& m1, const GSmatrix3& m2 );
	friend const GSmatrix3 operator * ( const GSmatrix3& m1, const GSmatrix3& m2 );
	friend const GSmatrix3 operator * ( const GSmatrix3& m, const GSfloat s );
	friend const GSmatrix3 operator * ( const GSfloat s, const GSmatrix3& m );
	friend bool operator == ( const GSmatrix3& v1, const GSmatrix3& v2 );
	friend bool operator != ( const GSmatrix3& v1, const GSmatrix3& v2 );

#endif

} GSmatrix3;

/*----- �S�s�S����W�ϊ��}�g���N�X�\���� ------------------------------------*/

typedef struct GSmatrix4
{
	union
	{
		/* �S�s�S��̍s��           */
		GSfloat		m[4][4];
		struct
		{
			GSfloat	_11, _12, _13, _14;
			GSfloat	_21, _22, _23, _24;
			GSfloat	_31, _32, _33, _34;
			GSfloat	_41, _42, _43, _44;
		};
		GSfloat		v[16];
	};

#ifdef __cplusplus

	/** �R���X�g���N�^ */
	GSmatrix4() {}
	GSmatrix4(
		GSfloat	m11, GSfloat m12, GSfloat m13, GSfloat m14,
		GSfloat	m21, GSfloat m22, GSfloat m23, GSfloat m24,
		GSfloat	m31, GSfloat m32, GSfloat m33, GSfloat m34,
		GSfloat	m41, GSfloat m42, GSfloat m43, GSfloat m44  );
	GSmatrix4(
		const GSvector4& axisX,
		const GSvector4& axisY,
		const GSvector4& axisZ,
		const GSvector4& position );
	explicit GSmatrix4( const GSfloat* V );
	explicit GSmatrix4( const GSmatrix3& M );
	explicit GSmatrix4( const GSquaternion& q );

	/** �P�ʍs�񉻂��� */
	GSmatrix4& identity();
	
	/** �s��̐��K���@*/
	GSmatrix4& normalize();
	/** �s��̐��K���@*/
	GSmatrix4 getNormalized() const;

	/** �g��k���s�����Z */
	GSmatrix4& scale( GSfloat x, GSfloat y, GSfloat z );

	/** ��]����Z */
	GSmatrix4& rotate( GSfloat angle, GSfloat axisX, GSfloat axisY, GSfloat axisZ );
	GSmatrix4& rotateYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll );

	/** ���s�ړ�����Z */
	GSmatrix4& translate( GSfloat x, GSfloat y, GSfloat z );

	/** �t�s��ɂ��� */
	GSmatrix4& inverse();

	/** �t�s��ɂ��� */
	GSmatrix4& inverseFast();

	/** �]�u�s��ɂ��� */
	GSmatrix4& transpose();

	/** �]�u�t�s��ɂ��� */
	GSmatrix4& inverseTranspose();

	/** �t�s����擾 */
	GSmatrix4 getInverse() const;

	/** �t�s����擾 */
	GSmatrix4 getInverseFast() const;

	/** �]�u�s����擾 */
	GSmatrix4 getTranspose() const;

	/** �]�u�t�s����擾 */
	GSmatrix4 getInverseTranspose() const;

	/** �X�P�[���l�̐ݒ� */
	GSmatrix4& setScale( GSfloat x, GSfloat y, GSfloat z );
	GSmatrix4& setScale( const GSvector3& scaleFactor );
	GSmatrix4& setScale( GSfloat scaleFactor );

	/** ��]�p�x�̐ݒ� */
	GSmatrix4& setRotation( GSfloat angle, GSfloat axisX, GSfloat axisY, GSfloat axisZ );
	GSmatrix4& setRotation( GSfloat angle, const GSvector3& axis );
	GSmatrix4& setRotation( const GSquaternion& q );
	GSmatrix4& setRotationX( GSfloat angle );
	GSmatrix4& setRotationY( GSfloat angle );
	GSmatrix4& setRotationZ( GSfloat angle );
	GSmatrix4& setRotationYawPitchRoll( GSfloat yaw, GSfloat pitch, GSfloat roll );

	/** ���s�ړ��ʂ̐ݒ� */
	GSmatrix4& setTranslation( GSfloat x, GSfloat y, GSfloat z );
	GSmatrix4& setTranslation( const GSvector3& translation );

	/** �����ϊ��s��̐ݒ� */
	GSmatrix4& setPerspectiveRH( GSfloat fovy, GSfloat aspect, GSfloat n, GSfloat f );
	GSmatrix4& setPerspectiveLH( GSfloat fovy, GSfloat aspect, GSfloat n, GSfloat f );
	GSmatrix4& setFrustumRH( GSfloat left, GSfloat right, GSfloat bottom, GSfloat top, GSfloat n, GSfloat f );
	GSmatrix4& setFrustumLH( GSfloat left, GSfloat right, GSfloat bottom, GSfloat top, GSfloat n, GSfloat f );
	GSmatrix4& setOrthoRH( GSfloat left, GSfloat right, GSfloat bottom, GSfloat top, GSfloat n = -1.0f, GSfloat f = 1.0f );
	GSmatrix4& setOrthoLH( GSfloat left, GSfloat right, GSfloat bottom, GSfloat top, GSfloat n = -1.0f, GSfloat f = 1.0f );

	/** ����ϊ��s��̐ݒ� */
	GSmatrix4& setLookAtRH( GSfloat fromX, GSfloat formY, GSfloat formZ,
						    GSfloat atX,   GSfloat atY,   GSfloat atZ,
						    GSfloat upX,   GSfloat upY,   GSfloat upZ );
	GSmatrix4& setLookAtRH( const GSvector3& from, const GSvector3& at, const GSvector3& up );
	GSmatrix4& setLookAtLH( GSfloat fromX, GSfloat formY, GSfloat formZ,
						    GSfloat atX,   GSfloat atY,   GSfloat atZ,
						    GSfloat upX,   GSfloat upY,   GSfloat upZ );
	GSmatrix4& setLookAtLH( const GSvector3& from, const GSvector3& at, const GSvector3& up );

	/** ����ϊ��s��ɕϊ����� */
	GSmatrix4& convertViewRH();
	GSmatrix4& convertViewLH();

	/** �n���ʃV���h�E�}�g���N�X�̐ݒ� */
	GSmatrix4& setShadow( const GSplane& plane, const GSvector3& lightPosition );

	/** ���W�ϊ� */
	GSvector2 transform( const GSvector2& v ) const;
	GSvector3 transform( const GSvector3& v ) const;
	GSvector4 transform( const GSvector4& v ) const;
	GSplane   transform( const GSplane& p ) const;
	GSvector2 transformCoord( const GSvector2& v ) const;
	GSvector3 transformCoord( const GSvector3& v ) const;
	GSvector4 transformCoord( const GSvector4& v ) const;
	GSvector3 transformNormal( const GSvector3& v ) const;

	/** ���C�ʒu�̐ݒ� */
	GSmatrix4& setAxisX( const GSvector3& axisX );
	GSmatrix4& setAxisY( const GSvector3& axisY );
	GSmatrix4& setAxisZ( const GSvector3& axisZ );
	GSmatrix4& setPosition( const GSvector3& position );

	/** ���[���E�s�b�`�E���[�̎擾 */
	GSfloat getRoll() const;
	GSfloat getPitch() const;
	GSfloat getYaw() const;

	/** �N�H�[�^�j�I�����擾 */
	GSquaternion getQuaternion() const;

	/** ���C�ʒu�C�X�P�[���C��]�̎擾 */
	GSvector3 getAxisX() const;
	GSvector3 getAxisY() const;
	GSvector3 getAxisZ() const;
	GSvector3 getPosition() const;
	GSvector3 getScale() const;
	GSmatrix4 getRotateMatrix() const;

	/** �O�����x�N�g���D��Ő��K�� */
	GSmatrix4& nomalizeAxisZ();
	/** ������x�N�g���D��Ő��K�� */
	GSmatrix4& nomalizeAxisY();
	/** �������x�N�g���D��Ő��K�� */
	GSmatrix4& nomalizeAxisX();

	/** �s��̐��`��ԁ@*/
	GSmatrix4 lerp(const GSmatrix4& other, float t) const;

	/** �L���X�g���Z�q�I�[�o�[���[�h */
	operator GSfloat* () const;
	operator const GSfloat* () const;

	/** �P�����Z�q�I�[�o�[���[�h */
	GSmatrix4 operator + () const;
	GSmatrix4 operator - () const;

	/** ������Z�q�I�[�o�[���[�h */
	GSmatrix4& operator += ( const GSmatrix4& v );
	GSmatrix4& operator -= ( const GSmatrix4& v );
	GSmatrix4& operator *= ( const GSmatrix4& v );
	GSmatrix4& operator *= ( GSfloat s );
	GSmatrix4& operator /= ( GSfloat s );

	/** �Q�����Z�q�I�[�o�[���[�h */
	friend const GSmatrix4 operator + ( const GSmatrix4& m1, const GSmatrix4& m2 );
	friend const GSmatrix4 operator - ( const GSmatrix4& m1, const GSmatrix4& m2 );
	friend const GSmatrix4 operator * ( const GSmatrix4& m1, const GSmatrix4& m2 );
	friend const GSmatrix4 operator * ( const GSmatrix4& m, const GSfloat s );
	friend const GSmatrix4 operator * ( const GSfloat s, const GSmatrix4& m );
	friend bool operator == ( const GSmatrix4& v1, const GSmatrix4& v2 );
	friend bool operator != ( const GSmatrix4& v1, const GSmatrix4& v2 );

	// --- Unity�݊��C���^�[�t�F�[�X�̒ǉ� 2020/04/13

	// �g��E�k��
	static GSmatrix4 scale(const GSvector3& v);
	// ��]
	static GSmatrix4 rotate(const GSquaternion& q);
	// ��]
	static GSmatrix4 rotateX(float degree);
	// ��]
	static GSmatrix4 rotateY(float degree);
	// ��]
	static GSmatrix4 rotateZ(float degree);
	// ���s�ړ�
	static GSmatrix4 translate(const GSvector3& v);
	// ���̍s����ړ��A��]��X�P�[�����O����s��ɐݒ肵�܂��B
	static GSmatrix4 TRS(const GSvector3& pos, const GSquaternion& q, const GSvector3& s);

	// ����ϊ��s��
	static GSmatrix4 lookAt(const GSvector3& eye, const GSvector3& at, const GSvector3& up);
	// �����ϊ��s��
	static GSmatrix4 perspective(float fov, float aspect, float zn, float zf);

	// ���W�ϊ��i�ʏ�j
	GSvector3 multiplyPoint(const GSvector3& point) const;
	// ���W�ϊ��i�����j
	GSvector3 multiplyPoint4x3(const GSvector3& point) const;
	// �x�N�g���ϊ��i���s�ړ��Ȃ��j
	GSvector3 multiplyVector(const GSvector3& point) const;

	// �X�P�[���l���擾
	GSvector3 scale() const;
	// ���W���擾
	GSvector3 position() const;
	// ��]�̎擾
	GSquaternion rotation() const;
	// �O�����x�N�g�����擾
	GSvector3 forward() const;
	// ������x�N�g�����擾
	GSvector3 up() const;
	// �������x�N�g�����擾
	GSvector3 left() const;

	// ���̍s��̋t�s��
	// GSmatrix4 inverse() const;
	// ���̍s��̓]�u�s��
	// GSmatrix4 transpose() const;
	// ���̍s����ړ��A��]��X�P�[�����O����s��ɐݒ肵�܂��B
	void setTRS(const GSvector3& pos, const GSquaternion& q, const GSvector3& s);

#endif

} GSmatrix4;

/*----- ��`�\���� ----------------------------------------------------------*/

typedef struct GSrect
{
	GSfloat			left;			/* ������̂����W           */
	GSfloat			top;			/* ������̂����W           */
	GSfloat			right;			/* �E�����̂����W           */
	GSfloat			bottom;			/* �E�����̂����W           */

#ifdef __cplusplus
	/** �R���X�g���N�^ */
	GSrect( GSfloat l = 0, GSfloat t = 0, GSfloat r = 0, GSfloat b = 0 ) :
		left( l ), top( t ), right( r ), bottom( b )
	{}
#endif

} GSrect;

/*----- �J���[�^ ------------------------------------------------------------*/

typedef	GSvector4	GScolor4;

#pragma warning(pop)

#endif

/********** End of File ******************************************************/
