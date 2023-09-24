#ifndef BASEDTO_H
#define BASEDTO_H

#include <QObject>
namespace dto
{
    class BaseDto
    {
        Q_GADGET

        Q_PROPERTY(int id READ id WRITE setId)

    public:
        virtual ~BaseDto();

        int id() const;
        virtual void setId(int id);

    private:
        int m_id = 0;
    };
}
#endif // BASEDTO_H
