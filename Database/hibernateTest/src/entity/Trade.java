package entity;

import javax.persistence.Basic;
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import java.sql.Timestamp;
import java.util.Objects;

@Entity
public class Trade {
    private int itemId;
    private Integer sellerId;
    private Integer buyerId;
    private Timestamp tradeTime;

    @Id
    @Column(name = "ItemID", nullable = false)
    public int getItemId() {
        return itemId;
    }

    public void setItemId(int itemId) {
        this.itemId = itemId;
    }

    @Basic
    @Column(name = "SellerID", nullable = true)
    public Integer getSellerId() {
        return sellerId;
    }

    public void setSellerId(Integer sellerId) {
        this.sellerId = sellerId;
    }

    @Basic
    @Column(name = "BuyerID", nullable = true)
    public Integer getBuyerId() {
        return buyerId;
    }

    public void setBuyerId(Integer buyerId) {
        this.buyerId = buyerId;
    }

    @Basic
    @Column(name = "TradeTime", nullable = false)
    public Timestamp getTradeTime() {
        return tradeTime;
    }

    public void setTradeTime(Timestamp tradeTime) {
        this.tradeTime = tradeTime;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Trade trade = (Trade) o;
        return itemId == trade.itemId &&
                Objects.equals(sellerId, trade.sellerId) &&
                Objects.equals(buyerId, trade.buyerId) &&
                Objects.equals(tradeTime, trade.tradeTime);
    }

    @Override
    public int hashCode() {
        return Objects.hash(itemId, sellerId, buyerId, tradeTime);
    }
}
