package entity;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import java.util.Objects;

@Entity
public class Market {
    private int itemId;
    private String name;
    private Object type;
    private Integer attribution;
    private Integer price;

    @Id
    @Column(name = "ItemID", nullable = false)
    public int getItemId() {
        return itemId;
    }

    public void setItemId(int itemId) {
        this.itemId = itemId;
    }

    @Basic
    @Column(name = "Name", nullable = true, length = 10)
    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    @Basic
    @Column(name = "Type", nullable = true)
    public Object getType() {
        return type;
    }

    public void setType(Object type) {
        this.type = type;
    }

    @Basic
    @Column(name = "Attribution", nullable = true)
    public Integer getAttribution() {
        return attribution;
    }

    public void setAttribution(Integer attribution) {
        this.attribution = attribution;
    }

    @Basic
    @Column(name = "Price", nullable = true)
    public Integer getPrice() {
        return price;
    }

    public void setPrice(Integer price) {
        this.price = price;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Market market = (Market) o;
        return itemId == market.itemId &&
                Objects.equals(name, market.name) &&
                Objects.equals(type, market.type) &&
                Objects.equals(attribution, market.attribution) &&
                Objects.equals(price, market.price);
    }

    @Override
    public int hashCode() {
        return Objects.hash(itemId, name, type, attribution, price);
    }
}
